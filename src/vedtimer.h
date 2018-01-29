
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

class VedTimer : public QObject
{
  Q_OBJECT
public:
  Q_INVOKABLE void resetTimer();
  Q_INVOKABLE void startTimer();
  Q_INVOKABLE QString setInterval(double tVal);
  Q_INVOKABLE void setCurrent(double tVal);
  Q_INVOKABLE void setVolume(double tVal);
  Q_PROPERTY(QString TimeToFill READ TimeToFill NOTIFY TimeToFillChanged)

  enum class FireStateType {
    STOP,PAUSED, BURNING, FILLHERUP
  };
  VedTimer();
  ~VedTimer();
  void UpdateValueText();

  QString TimeToFill()
  {
    return m_sTimeToFill;
  }

  void SetCurrentValObj(QObject*p) {
    m_pCurrentValObj = p;
  }

  void SetStartBtnTextObj(QObject*p) {
    m_pStartBtnTextObj = p;
  }

  void SetFireState(FireStateType e);
signals:
  void TimeToFillChanged();

private:
  int m_nInterval;
  int m_nCurrent;
  QString m_sTimeToFill;
  QObject* m_pCurrentValObj;
  QObject* m_pStartBtnTextObj;
  MssTimer m_oSecTimer;
  FireStateType m_eFireState;
  QSoundEffect* m_oEffect;
  MssWorkerThread m_oThread;
  int m_nLastTime;
  void *  m_iphbdHandler ;

};


