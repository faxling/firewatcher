#include "VedTimer.h"
#include "time.h"
#include "QVariant"
#include <QSoundEffect>
#include <sailfishapp.h>


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
  m_pCurrentTimeTextObj = 0;
  m_nCurrent = 0;
  m_pStartBtnTextObj = 0;
  m_oEffect = new QSoundEffect(this);
  m_oEffect->setSource(QUrl("qrc:/44-04.wav"));

  m_oSecTimer.SetTimeOut([&] {

    if (m_eFireState == FireStateType::PAUSED || m_eFireState == FireStateType::STOP)
      return;
    --m_nCurrent;

    if (m_nCurrent <= 0)
    {
      if (m_oEffect->isPlaying()==false)
        m_oEffect->play();

      m_pCurrentTimeTextObj->setProperty("text", FormatDuration(-m_nCurrent));
      SetFireState(FireStateType::FILLHERUP);
    }
    else
      m_pCurrentTimeTextObj->setProperty("text", FormatDuration(m_nCurrent));

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


void VedTimer::setVolume(double (tVal))
{
  m_oEffect->setVolume(tVal);
}

void VedTimer::resetTimer()
{
  m_nCurrent = m_nInterval;
  m_pCurrentTimeTextObj->setProperty("text", FormatDuration(m_nCurrent));
  SetFireState(FireStateType::STOP);
}



void VedTimer::setCurrent(double tVal)
{

}

void VedTimer::startTimer(void)
{
  if (m_eFireState == FireStateType::STOP)
  {
    m_nCurrent = m_nInterval;
    m_pCurrentTimeTextObj->setProperty("text", FormatDuration(m_nCurrent));
    SetFireState(FireStateType::BURNING);
    return;
  }
  if (m_eFireState == FireStateType::PAUSED)
  {
    SetFireState(FireStateType::BURNING);
    return;
  }

  if (m_eFireState == FireStateType::BURNING)
  {
    SetFireState(FireStateType::PAUSED);
    return;
  }
  if (m_eFireState == FireStateType::FILLHERUP)
  {
    SetFireState(FireStateType::BURNING);
    m_nCurrent = m_nInterval;
    m_pCurrentTimeTextObj->setProperty("text", FormatDuration(m_nCurrent));
  }
}

QString VedTimer::setInterval(double tVal)
{
  m_nInterval = tVal * 7200;
  m_nInterval = (m_nInterval / 60) * 60;
  if (m_eFireState == FireStateType::STOP)
  {
    m_nCurrent = m_nInterval;
    if (m_pCurrentTimeTextObj != nullptr)
      m_pCurrentTimeTextObj->setProperty("text", FormatDuration(m_nCurrent));
  }
  return FormatDuration(m_nInterval);
}



VedTimer::~VedTimer()
{

}
