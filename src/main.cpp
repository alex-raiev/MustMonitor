#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "modbusclient.h"
#include "dataviewmodel.h"
#include "settingsmanager.h"
#include "appconfig.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    QString configPath = AppConfig::findConfigPath(argc, argv);
    if (configPath.isEmpty()) {
        return -1;
    }
    
    ModbusClient modbusClient;
    DataViewModel viewModel;
    SettingsManager settingsManager;
    
    QObject::connect(&modbusClient, &ModbusClient::dataReceived,
                     &viewModel, &DataViewModel::updateData);
    
    QObject::connect(&modbusClient, &ModbusClient::registerNamesLoaded,
                     &viewModel, &DataViewModel::setRegisterNames);
    
    QObject::connect(&modbusClient, &ModbusClient::connectionStateChanged,
                     &settingsManager, &SettingsManager::setConnected);
    
    QObject::connect(&settingsManager, &SettingsManager::settingsApplied,
                     &modbusClient, &ModbusClient::reconnect);
    
    if (!modbusClient.loadConfig(configPath)) {
        qWarning() << "Failed to load config from:" << configPath;
        return -1;
    }
    
    settingsManager.loadFromConfig(configPath);
    
    modbusClient.connectToServer();
    
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dataViewModel", &viewModel);
    engine.rootContext()->setContextProperty("modbusClient", &modbusClient);
    engine.rootContext()->setContextProperty("settingsManager", &settingsManager);
    
    engine.loadFromModule("MustMonitor", "Main");
    if (engine.rootObjects().isEmpty())
        return -1;
    
    return app.exec();
}
