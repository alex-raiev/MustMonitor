#include "modbusclient.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>

ModbusClient::ModbusClient(QObject *parent)
    : QObject(parent)
    , m_ctx(nullptr)
    , m_pollTimer(new QTimer(this))
    , m_port(502)
    , m_deviceId(1)
{
    connect(m_pollTimer, &QTimer::timeout, this, &ModbusClient::readRegisters);
}

ModbusClient::~ModbusClient() {
    if (m_ctx) {
        modbus_close(m_ctx);
        modbus_free(m_ctx);
    }
}

bool ModbusClient::loadConfig(const QString& configPath) {
    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open config file:" << configPath;
        qWarning() << "Error:" << file.errorString();
        return false;
    }
    
    QByteArray jsonData = file.readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return false;
    }
    
    QJsonObject root = doc.object();
    
    QJsonObject modbus = root["modbus"].toObject();
    m_host = modbus["host"].toString();
    m_port = modbus["port"].toInt(502);
    m_deviceId = modbus["deviceId"].toInt(1);
    m_pollTimer->setInterval(modbus["pollInterval"].toInt(5000));
    
    QJsonArray registers = root["registers"].toArray();
    for (const QJsonValue& val : registers) {
        QJsonObject reg = val.toObject();
        RegisterConfig config;
        config.name = reg["name"].toString();
        config.address = reg["address"].toInt();
        config.scaleFactor = reg["scale"].toDouble(1.0);
        config.isSigned = reg["isSigned"].toBool(false);
        m_registers.append(config);
    }
    
    qDebug() << "Loaded" << m_registers.size() << "registers from config";
    
    emit registerNamesLoaded(
        getRegisterName(113),
        getRegisterName(25216),
        getRegisterName(25206),
        getRegisterName(25210),
        getRegisterName(25207),
        getRegisterName(25211),
        getRegisterName(25226),
        getRegisterName(25225),
        getRegisterName(111),
        getRegisterName(25215),
        getRegisterName(25213)
    );
    
    return true;
}

QString ModbusClient::getRegisterName(quint16 address) const {
    for (const RegisterConfig& reg : m_registers) {
        if (reg.address == address) {
            return reg.name;
        }
    }
    return QString();
}

void ModbusClient::connectToServer() {
    if (m_ctx) {
        modbus_close(m_ctx);
        modbus_free(m_ctx);
    }
    
    m_ctx = modbus_new_tcp(m_host.toUtf8().constData(), m_port);
    if (!m_ctx) {
        qWarning() << "Failed to create modbus context";
        emit connectionStateChanged(false);
        return;
    }
    
    modbus_set_response_timeout(m_ctx, 1, 0);
    modbus_set_slave(m_ctx, m_deviceId);
    
    qDebug() << "Connecting to" << m_host << ":" << m_port << "device ID:" << m_deviceId;
    if (modbus_connect(m_ctx) == -1) {
        qWarning() << "Connection failed:" << modbus_strerror(errno);
        modbus_free(m_ctx);
        m_ctx = nullptr;
        emit connectionStateChanged(false);
        return;
    }
    
    qDebug() << "Modbus connected to" << m_host << ":" << m_port;
    emit connectionStateChanged(true);
    m_pollTimer->start();
}

void ModbusClient::reconnect(const QString& host, int port, int pollInterval) {
    disconnect();
    m_host = host;
    m_port = port;
    m_pollTimer->setInterval(pollInterval);
    qDebug() << "Reconnecting to" << m_host << ":" << m_port;
    connectToServer();
}

void ModbusClient::disconnect() {
    m_pollTimer->stop();
    if (m_ctx) {
        modbus_close(m_ctx);
        modbus_free(m_ctx);
        m_ctx = nullptr;
    }
    qDebug() << "Modbus disconnected";
    emit connectionStateChanged(false);
}

void ModbusClient::readRegisters() {
    if (!m_ctx)
        return;
    
    for (const RegisterConfig& reg : m_registers) {
        uint16_t value;
        qDebug() << "request: addr=" << reg.address;
        
        if (modbus_read_registers(m_ctx, reg.address, 1, &value) == -1) {
            qWarning() << "Read failed for" << reg.name << ":" << modbus_strerror(errno);
            continue;
        }
        
        double scaledValue = reg.isSigned ? 
            static_cast<int16_t>(value) * reg.scaleFactor : 
            value * reg.scaleFactor;
        m_values[reg.address] = scaledValue;
        qDebug() << "response: addr=" << reg.address << reg.name 
                 << "raw=" << value << "scaled=" << scaledValue;
        
        QThread::msleep(100);
    }
    
    if (m_values.size() == m_registers.size()) {
        emit dataReceived(
            m_values.value(113, 0.0),
            m_values.value(25216, 0.0),
            m_values.value(25206, 0.0),
            m_values.value(25210, 0.0),
            m_values.value(25207, 0.0),
            m_values.value(25211, 0.0),
            m_values.value(25226, 0.0),
            m_values.value(25225, 0.0),
            m_values.value(111, 0.0),
            m_values.value(25215, 0.0),
            m_values.value(25213, 0.0)
        );
    }
}
