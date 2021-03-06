/*

*/


#include "vedtimer.h"
#include <QQuickView>
#include <QGuiApplication>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlContext>
#include <sailfishapp.h>

#include "memory"

#include <QDebug>


int main(int argc, char *argv[])
{
  QGuiApplication *app = SailfishApp::application(argc, argv);
  QQuickView *view = SailfishApp::createView();

  // std::unique_ptr<VedTimer> pVedTimer(new VedTimer);
  VedTimer* pVedTimer(new VedTimer);
  QString qml = "qml/harbour-frax-firewatcher.qml";

  QQmlContext* p = view->rootContext();
  p->setContextProperty("VedTimer", pVedTimer);
  view->setSource(SailfishApp::pathTo(qml));

  pVedTimer->SetVolumeSliderObj(view->rootObject()->findChild<QObject*>("idSliderVolume"));
  pVedTimer->SetIntervallSliderObj(view->rootObject()->findChild<QObject*>("idSliderInterval"));
  pVedTimer->SetCurrentSliderValObj(view->rootObject()->findChild<QObject*>("idSliderCurrentVal"));
  pVedTimer->SetStartBtnTextObj(view->rootObject()->findChild<QObject*>("idStartBtn"));
  pVedTimer->SetAppObj(view->rootObject());
  view->show();

  QObject::connect(view->engine(), &QQmlEngine::quit,
                   app, &QGuiApplication::quit);

  app->exec();

  return 1;
}

