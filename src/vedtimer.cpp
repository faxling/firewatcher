#include "VedTimer.h"
#include "time.h"
#include "QVariant"
#include <QSoundEffect>
#include <QSettings>
#include <sailfishapp.h>

#ifdef USE_FEEDBACK
#include <QtFeedback/QFeedbackHapticsEffect>
#endif

extern "C" {
#include "libiphb/libiphb.h"
}


MssTimer::MssTimer()
{
  m_pTimer = new QBasicTimer;
}

MssTimer::MssTimer(std::function<void()> pfnTimeOut)
{
  m_pTimer = new QBasicTimer;
  m_pfnTimeOut = pfnTimeOut;
  m_bIsSingleShot = false;
}

MssTimer::~MssTimer()
{
  delete m_pTimer;
}


void MssTimer::Start(int nMilliSec)
{
  m_bIsSingleShot = false;
  m_pTimer->start(nMilliSec, this);
}

void MssTimer::SingleShot(int nMilliSec)
{
  m_bIsSingleShot = true;
  if (m_pTimer->isActive() == true)
    m_pTimer->stop();
  m_pTimer->start(nMilliSec, this);
}

void MssTimer::Stop()
{
  m_pTimer->stop();
}

bool MssTimer::IsActive() {
  return m_pTimer->isActive();
}

void MssTimer::timerEvent(QTimerEvent *)
{
  if (m_pfnTimeOut != nullptr)
    m_pfnTimeOut();

  if (m_bIsSingleShot == true)
    m_pTimer->stop();
}

QString FormatDuration(int nTime)
{
  if (nTime<0)
    nTime = -nTime;
  wchar_t szStr[20];
  time_t now = nTime;

  tm *tmNow = gmtime(&now);

  if (nTime < 0)
    wcscpy(szStr, L"-");
  else if (tmNow->tm_hour > 0)
    wcsftime(szStr, 20, L"%H:%M:%S", tmNow);
  else
    wcsftime(szStr, 20, L"%M:%S", tmNow);

  QString sRet;
  sRet.setUtf16((const ushort*)szStr, wcslen(szStr)*2);
  return sRet;
}


VedTimer::VedTimer()
  : QObject()
{
  m_nInterval = 60;
  m_nCurrent = 0;
  m_nStartTime = 0;
  m_pStartBtnTextObj = 0;
  m_fElapsedTimeSliderValue = 0;
  m_pCurrentValObj = nullptr;
  m_oEffect = new QSoundEffect(this);
  m_oEffect->setSource(QUrl("qrc:/44-04-2.wav"));
  m_oEffect->setLoopCount(QSoundEffect::Infinite);

#ifdef USE_FEEDBACK
  m_pFeedback = new QFeedbackHapticsEffect(this);
  m_pFeedback->setDuration(1000);
#endif

  m_pSettings = new QSettings("frax","Firewatcher",this);
  m_oThread.Set([&] {
    m_iphbdHandler =  iphb_open(0);
    for (;;)
    {

      QThread::msleep(1000);
      if (m_eFireState != FireStateType::BURNING)
        return;

      iphb_wait(m_iphbdHandler,m_nCurrent - 10, m_nCurrent , 1);

      if (m_oSecTimer.IsActive() == false)
        m_oSecTimer.Start(1000);

    }
  });

  m_oThread.start();

  m_oSecTimer.SetTimeOut([&] {
    UpdateTotalText();
    int nNow = time(0);
    if (m_eFireState == FireStateType::PAUSED || m_eFireState == FireStateType::STOP)
    {
      m_nLastTime = nNow;
      return;
    }
    iphb_discard_wakeups(m_iphbdHandler);
    if ( m_nLastTime == nNow)
      return;
    m_nCurrent -=(nNow-m_nLastTime);
    m_nLastTime = nNow;

    if (m_nCurrent <= 0)
    {
      if (m_oEffect->isPlaying()==true )
      {

#ifdef USE_FEEDBACK
        if ((m_nCurrent % 2) == 0)
          m_pFeedback->start();
#endif
      }
      if (m_oEffect->isPlaying()==false)
      {
        m_oEffect->play();
      }

      SetFireState(FireStateType::FILLHERUP);
    }

    // update of value gives a call to setCurrent that updates the slider tex by UpdateValueText()
    if ( m_nCurrent > 0)
      SetCurrentSliderVal((double)m_nCurrent / m_nInterval);
    else
    {
      if (m_nCurrent == 0)
        SetCurrentSliderVal(0);
      UpdateValueText();
    }

  });

  m_eFireState = FireStateType::STOP;
  m_oSecTimer.Start(1000);

  // Init Of Sliders in Qml
}

void VedTimer::SetFireState(FireStateType e) {
  if (m_eFireState == e)
    return;
  m_eFireState = e;
  m_pStartBtnTextObj->setProperty("nFireState", int(m_eFireState));
}


void VedTimer::SetCurrentSliderVal(double fVal)
{
  if (m_pCurrentValObj == nullptr)
    return;
  m_pCurrentValObj->setProperty("value",fVal);
}

double VedTimer::ElapsedTimeSliderValue()
{
  return m_fElapsedTimeSliderValue;

}

QString VedTimer::TimeToFillStr()
{
  return m_sTimeToFillStr;
}
QString VedTimer::IntervallStr()
{
  return m_sIntervallStr;
}
QString VedTimer::TotalStr()
{
  return m_sTotalStr;
}

void VedTimer::setCurrent(double fValue) {
  m_nCurrent = fValue *  m_nInterval;
  UpdateValueText();
}

void VedTimer::SetIntervallSliderObj(QObject*p) {
  m_pIntervallSliderObj = p;
  double fVal = m_pSettings->value("intervall",0.2).toDouble();
  m_pIntervallSliderObj->setProperty("value",fVal);
  setInterval(fVal);
}

void VedTimer::setInterval(double fVal)
{
  m_pSettings->setValue("intervall",fVal);

  m_nInterval = fVal * 7200;
  m_nInterval = (m_nInterval / 60) * 60;
  if (m_nInterval < 60)
    m_nInterval = 60;

  if (m_eFireState == FireStateType::STOP)
  {
    m_nCurrent = m_nInterval;
    UpdateValueText();
  }

  SetCurrentSliderVal((double)m_nCurrent / m_nInterval);
  UpdateIntervalText();

}

void VedTimer::setVolume(double (tVal))
{
  m_oEffect->setVolume(tVal);
}

void VedTimer::UpdateTotalText()
{
  QString sVal;
  int nElapsed = 0 ;
  switch (m_eFireState)
  {
  case FireStateType::BURNING:
  case FireStateType::FILLHERUP:
  case FireStateType::PAUSED:
    nElapsed = time(0) - m_nStartTime ;
    sVal = FormatDuration(nElapsed);
    break;
  case FireStateType::STOP:
    sVal = FormatDuration(0);
  }

  if (nElapsed != 0)
  {
    int nNrIntervalsElapsed = nElapsed / m_nInterval;
    double fScale = (nNrIntervalsElapsed+1)* m_nInterval;
    m_fElapsedTimeSliderValue = nElapsed / fScale ;
  } else
    m_fElapsedTimeSliderValue = 0;

  if (sVal != m_sTotalStr)
  {
    m_sTotalStr = sVal;
    emit TotalStrChanged();
    emit ElapsedTimeSliderValueChanged();
  }
}

void VedTimer::UpdateIntervalText()
{
  m_sIntervallStr = FormatDuration(m_nInterval);
  emit IntervallStrChanged();
}

void VedTimer::UpdateValueText()
{
  m_sTimeToFillStr = FormatDuration(m_nCurrent);
  emit TimeToFillStrChanged();
}

void VedTimer::resetTimer()
{
#ifdef USE_FEEDBACK
  m_pFeedback->stop();
#endif

  m_oEffect->stop();
  m_nCurrent = m_nInterval;
  SetCurrentSliderVal(1);
  UpdateValueText();

  SetFireState(FireStateType::STOP);
  UpdateTotalText();
}

void VedTimer::startTimer(void)
{
  m_oEffect->stop();
  switch(m_eFireState)
  {
  case FireStateType::STOP:
    m_nStartTime = time(0);
    SetFireState(FireStateType::BURNING);
    return;
  case FireStateType::PAUSED:
    SetFireState(FireStateType::BURNING);
    return;
  case FireStateType::BURNING:
    SetFireState(FireStateType::PAUSED);
    return;
  case FireStateType::FILLHERUP:
    SetFireState(FireStateType::BURNING);
    SetCurrentSliderVal(1);
  }
}

VedTimer::~VedTimer()
{


}
