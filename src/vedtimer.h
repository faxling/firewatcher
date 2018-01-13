
#include <QtCore/QObject>
#include <QtCore/QBasicTimer>
#include <functional>

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


class QSoundEffect;

class VedTimer : public QObject
{
  Q_OBJECT
public:
  Q_INVOKABLE void resetTimer();
  Q_INVOKABLE void startTimer();
  Q_INVOKABLE QString setInterval(double tVal);
  Q_INVOKABLE QString setCurrent(double tVal);
  Q_INVOKABLE void setVolume(double tVal);
  enum class FireStateType {
    STOP,PAUSED, BURNING, FILLHERUP
  };
  VedTimer();
  ~VedTimer();

  void SetCurrentTimeTextObj(QObject*p) {
    m_pCurrentTimeTextObj = p;
  }
  void SetStartBtnTextObj(QObject*p) {
    m_pStartBtnTextObj = p;
  }
  void SetFireState(FireStateType e);

private:
  int m_nInterval;
  int m_nCurrent;

  QObject* m_pCurrentTimeTextObj;
  QObject* m_pStartBtnTextObj;
  MssTimer m_oSecTimer;
  FireStateType m_eFireState;
  QSoundEffect* m_oEffect;
};


