#pragma once
#include <QObject>
#include <QTimer>
#include <QMap>
#include <modbus/modbus.h>

struct RegisterConfig {
    QString name;
    quint16 address;
    double scaleFactor;
    bool isSigned;
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
    
signals:
    void dataReceived(double batterySoc, double loadPowerPercent, double invertorVoltage,
                      double invertorCurrent, double gridVoltage, double gridCurrent,
                      double gridFrequency, double invertorFrequency);
    void connectionStateChanged(bool connected);
    void registerNamesLoaded(QString batterySoc, QString loadPowerPercent,
                            QString invertorVoltage, QString invertorCurrent,
                            QString gridVoltage, QString gridCurrent,
                            QString gridFrequency, QString invertorFrequency);
    
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
};
