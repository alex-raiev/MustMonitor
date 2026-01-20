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
    
    QObject::connect(&modbusClient, &ModbusClient::connectionStateChanged,
                     &settingsManager, &SettingsManager::setConnected);
    
    QObject::connect(&settingsManager, &SettingsManager::settingsApplied,
                     &modbusClient, &ModbusClient::reconnect);
    
    QObject::connect(&modbusClient, &ModbusClient::registerNamesLoaded,
                     &viewModel, &DataViewModel::setRegisterNames);
    
    if (!modbusClient.loadConfig(configPath)) {
        qWarning() << "Failed to load config from:" << configPath;
        return -1;
    }
    
    settingsManager.loadFromConfig(configPath);
    
    QThread modbusThread;
    modbusClient.moveToThread(&modbusThread);
    modbusThread.start();
    
    QMetaObject::invokeMethod(&modbusClient, &ModbusClient::connectToServer, Qt::QueuedConnection);
    
    QTimer dataTimer;
    QObject::connect(&dataTimer, &QTimer::timeout, [&]() {
        auto* queue = modbusClient.getDataQueue();
        if (!queue->isEmpty()) {
            ModbusData data = queue->pop();
            viewModel.updateData(data.batterySoc, data.loadPowerPercent, data.invertorVoltage,
                               data.invertorCurrent, data.gridVoltage, data.gridCurrent,
                               data.gridFrequency, data.invertorFrequency, data.batteryTemperature,
                               data.loadPowerKw, data.inverterPowerKva);
        }
    });
    dataTimer.start(100);
    
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dataViewModel", &viewModel);
    engine.rootContext()->setContextProperty("modbusClient", &modbusClient);
    engine.rootContext()->setContextProperty("settingsManager", &settingsManager);
    
    engine.loadFromModule("MustMonitor", "Main");
    if (engine.rootObjects().isEmpty()) {
        modbusThread.quit();
        modbusThread.wait();
        return -1;
    }
    
    int result = app.exec();
    
    dataTimer.stop();
    modbusClient.disconnect();
    modbusThread.quit();
    modbusThread.wait();
    
    return result;
}
