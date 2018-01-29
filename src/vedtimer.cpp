#include "VedTimer.h"
#include "time.h"
#include "QVariant"
#include <QSoundEffect>
#include <sailfishapp.h>

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
  m_pCurrentValObj = 0;
  m_nCurrent = 0;
  m_pStartBtnTextObj = 0;
  m_oEffect = new QSoundEffect(this);
  m_oEffect->setSource(QUrl("qrc:/44-04-2.wav"));
  m_oEffect->setLoopCount(QSoundEffect::Infinite);

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
      if (m_oEffect->isPlaying()==false)
        m_oEffect->play();

      SetFireState(FireStateType::FILLHERUP);
    }

    if (m_pCurrentValObj != nullptr)
    {
      if ( m_nCurrent > 0)
        m_pCurrentValObj->setProperty("value",(double)m_nCurrent / m_nInterval);
      else
      {
        if (m_pCurrentValObj->property("value").toInt() != 0)
          m_pCurrentValObj->setProperty("value",0);
        else
          UpdateValueText();
      }
    }


  });

  m_eFireState = FireStateType::STOP;
  m_oSecTimer.Start(1000);

}

void VedTimer::SetFireState(FireStateType e) {
  if (m_eFireState == e)
    return;
  m_eFireState = e;
  m_pStartBtnTextObj->setProperty("nFireState", int(m_eFireState));
}

void VedTimer::UpdateValueText()
{
  m_sTimeToFill = FormatDuration(m_nCurrent);
  emit TimeToFillChanged();
}

void VedTimer::setVolume(double (tVal))
{
  m_oEffect->setVolume(tVal);
}

void VedTimer::resetTimer()
{
  m_nCurrent = m_nInterval;
  UpdateValueText();
  SetFireState(FireStateType::STOP);
}



void VedTimer::setCurrent(double tVal)
{
  m_nCurrent = tVal *  m_nInterval;
  UpdateValueText();
}

void VedTimer::startTimer(void)
{

  m_oEffect->stop();
  switch(m_eFireState)
  {
  case FireStateType::STOP:
    // m_nCurrent = m_nInterval;
    UpdateValueText();
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
    m_nCurrent = m_nInterval;
    UpdateValueText();
  }
}

QString VedTimer::setInterval(double tVal)
{
  m_nInterval = tVal * 7200;
  m_nInterval = (m_nInterval / 60) * 60;
  if (m_nInterval < 60)
    m_nInterval = 60;

  if (m_eFireState == FireStateType::STOP)
  {
    m_nCurrent = m_nInterval;
  }
  UpdateValueText();

  return FormatDuration(m_nInterval);
}


VedTimer::~VedTimer()
{

}
