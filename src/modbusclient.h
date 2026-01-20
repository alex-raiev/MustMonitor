#pragma once
#include <QObject>
#include <QTimer>
#include <QMap>
#include <QThread>
#include <modbus/modbus.h>
#include "threadsafequeue.h"

struct RegisterConfig {
    QString name;
    quint16 address;
    double scaleFactor;
    bool isSigned;
};

struct ModbusData {
    double batterySoc;
    double loadPowerPercent;
    double invertorVoltage;
    double invertorCurrent;
    double gridVoltage;
    double gridCurrent;
    double gridFrequency;
    double invertorFrequency;
    double batteryTemperature;
    double loadPowerKw;
    double inverterPowerKva;
};

class ModbusClient : public QObject {
    Q_OBJECT
    
public:
    explicit ModbusClient(QObject *parent = nullptr);
    ~ModbusClient();
    
    bool loadConfig(const QString& configPath);
    void connectToServer();
    Q_INVOKABLE void reconnect(const QString& host, int port, int pollInterval);
    Q_INVOKABLE void disconnect();
    QString getRegisterName(quint16 address) const;
    ThreadSafeQueue<ModbusData>* getDataQueue() { return &m_dataQueue; }
    
signals:
    void connectionStateChanged(bool connected);
    void registerNamesLoaded(QString batterySoc, QString loadPowerPercent,
                            QString invertorVoltage, QString invertorCurrent,
                            QString gridVoltage, QString gridCurrent,
                            QString gridFrequency, QString invertorFrequency,
                            QString batteryTemperature, QString loadPowerKw,
                            QString inverterPowerKva);
    
private slots:
    void readRegisters();
    
private:
    modbus_t* m_ctx;
    QTimer* m_pollTimer;
    QList<RegisterConfig> m_registers;
    QMap<quint16, double> m_values;
    QString m_host;
    int m_port;
    int m_deviceId;
    ThreadSafeQueue<ModbusData> m_dataQueue;
};
