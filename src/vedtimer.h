
#include <QtCore/QObject>
#include <QtCore/QBasicTimer>
#include <QtCore/QThread>
#include <functional>
// #include "libiphb/libiphb.h"

class MssTimer : public QObject
{
public:
  MssTimer(std::function<void()> pfnTimeOut);
  MssTimer();
  ~MssTimer();
  void SetTimeOut(std::function<void()> pfnTimeOut) {
    m_pfnTimeOut = pfnTimeOut;
  }
  void Start(int nMilliSec);
  void SingleShot(int nMilliSec);
  void Stop();
  bool IsActive();
private:
  void timerEvent(QTimerEvent *pEvent);
  QBasicTimer* m_pTimer;
  std::function<void()> m_pfnTimeOut;
  bool m_bIsSingleShot;

};

class MssWorkerThread : public QThread
{

public:
  MssWorkerThread(std::function<void()> pfMain)
  {
    m_pfMain = pfMain;
  }
  MssWorkerThread()
  {
    m_pfMain = nullptr;
  }
  void Set(std::function<void()> pfMain)
  {
    m_pfMain = pfMain;
  }
private:
  virtual void run() {
    if (m_pfMain != nullptr)
      m_pfMain();
  }

  std::function<void()> m_pfMain;
};


class QSoundEffect;
class QFeedbackHapticsEffect;
class QSettings;
class VedTimer : public QObject
{
  Q_OBJECT
public:
  Q_INVOKABLE void resetTimer();
  Q_INVOKABLE void startTimer();
  Q_INVOKABLE void setVolume(double tVal);
  Q_INVOKABLE void setInterval(double tVal);
  Q_INVOKABLE void setCurrent(double tVal);
  Q_PROPERTY(QString TimeToFillStr READ TimeToFillStr NOTIFY TimeToFillStrChanged)
  Q_PROPERTY(QString IntervallStr READ IntervallStr NOTIFY IntervallStrChanged)
  Q_PROPERTY(QString TotalStr READ TotalStr NOTIFY TotalStrChanged)
  Q_PROPERTY(double ElapsedTimeSliderValue READ ElapsedTimeSliderValue NOTIFY ElapsedTimeSliderValueChanged)
  enum class FireStateType {
    STOP,PAUSED, BURNING, FILLHERUP
  };
  VedTimer();
  ~VedTimer();

  // QML Properties
  QString TimeToFillStr();
  QString IntervallStr();
  QString TotalStr();
  void SetCurrentSliderVal(double fVal);
  void UpdateValueText();
  void UpdateIntervalText();
  void UpdateTotalText();
  double ElapsedTimeSliderValue();
  void SetCurrentValObj(QObject*p) {
    m_pCurrentValObj = p;
  }

  void SetIntervallSliderObj(QObject*p);

  void SetStartBtnTextObj(QObject*p) {
    m_pStartBtnTextObj = p;
  }

  void SetFireState(FireStateType e);

signals:
  void TimeToFillStrChanged();
  void IntervallStrChanged();
  void TotalStrChanged();
  void ElapsedTimeSliderValueChanged();

private:
  int m_nInterval;
  int m_nCurrent;
  int m_nStartTime;
  double m_fElapsedTimeSliderValue;

  QObject* m_pStartBtnTextObj;
  QObject* m_pCurrentValObj;
  QObject* m_pIntervallSliderObj;
  QFeedbackHapticsEffect* m_pFeedback;
  QString m_sIntervallStr;
  QString m_sTimeToFillStr;
  QString m_sTotalStr;
  MssTimer m_oSecTimer;
  FireStateType m_eFireState;
  QSoundEffect* m_oEffect;
  QSettings* m_pSettings;
  MssWorkerThread m_oThread;
  int m_nLastTime;
  void *  m_iphbdHandler ;
};


