// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dbus/appearance1adaptor.h"
#include "dbus/appearance1.h"
#include "wmadaptor.h"
#include "dbus/deepinwmfaker.h"
#include "modules/common/commondefine.h"

#include <DLog>

#include <QGuiApplication>

using Dtk::Core::DLogManager;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("org.deepin.dde.appearance");
    QTranslator translator;
    QString languagePath = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                  QString("dde-appearance/translations"),
                                                  QStandardPaths::LocateDirectory);
    translator.load(languagePath+"/dde-appearance_" + QLocale::system().name());
    app.installTranslator(&translator);

    DLogManager::registerJournalAppender();
    DLogManager::registerConsoleAppender();
 
    Appearance1 *appearance = new Appearance1();
    new Appearance1Adaptor(appearance);

    APPEARANCEDBUS.registerService(AppearanceService);
    APPEARANCEDBUS.registerObject(AppearancePath, AppearanceInterface, appearance);

    DeepinWMFaker faker;
    WmAdaptor wmAdaptor(&faker);
    QDBusConnection::sessionBus().registerService("com.deepin.wm");
    QDBusConnection::sessionBus().registerObject("/com/deepin/wm", "com.deepin.wm", &faker);

    return app.exec();
}
