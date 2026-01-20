#include "settingsmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
    , m_port(502)
    , m_pollInterval(1000)
    , m_connected(false)
{
}

void SettingsManager::setHost(const QString& host) {
    if (m_host != host) {
        m_host = host;
        emit hostChanged();
    }
}

void SettingsManager::setPort(int port) {
    if (m_port != port) {
        m_port = port;
        emit portChanged();
    }
}

void SettingsManager::setPollInterval(int interval) {
    if (m_pollInterval != interval) {
        m_pollInterval = interval;
        emit pollIntervalChanged();
    }
}

void SettingsManager::setConnected(bool connected) {
    if (m_connected != connected) {
        m_connected = connected;
        emit connectedChanged();
    }
}

bool SettingsManager::loadFromConfig(const QString& configPath) {
    m_configPath = configPath;
    
    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open config file:" << configPath;
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();
    QJsonObject modbus = root["modbus"].toObject();
    
    setHost(modbus["host"].toString("192.168.1.43"));
    setPort(modbus["port"].toInt(502));
    setPollInterval(modbus["pollInterval"].toInt(1000));
    
    return true;
}

bool SettingsManager::saveConfig() {
    if (m_configPath.isEmpty()) {
        qWarning() << "Config path not set";
        return false;
    }
    
    QFile file(m_configPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    
    QJsonObject root = doc.object();
    QJsonObject modbus = root["modbus"].toObject();
    
    modbus["host"] = m_host;
    modbus["port"] = m_port;
    modbus["pollInterval"] = m_pollInterval;
    
    root["modbus"] = modbus;
    doc.setObject(root);
    
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot write config file:" << m_configPath;
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    qDebug() << "Config saved successfully";
    return true;
}

void SettingsManager::applySettings() {
    emit settingsApplied(m_host, m_port, m_pollInterval);
}
