#pragma once
#include <QObject>
#include <QString>

class SettingsManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int pollInterval READ pollInterval WRITE setPollInterval NOTIFY pollIntervalChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    
public:
    explicit SettingsManager(QObject *parent = nullptr);
    
    QString host() const { return m_host; }
    int port() const { return m_port; }
    int pollInterval() const { return m_pollInterval; }
    bool connected() const { return m_connected; }
    
    void setHost(const QString& host);
    void setPort(int port);
    void setPollInterval(int interval);
    void setConnected(bool connected);
    
    bool loadFromConfig(const QString& configPath);
    Q_INVOKABLE bool saveConfig();
    Q_INVOKABLE void applySettings();
    
signals:
    void hostChanged();
    void portChanged();
    void pollIntervalChanged();
    void connectedChanged();
    void settingsApplied(QString host, int port, int pollInterval);
    
private:
    QString m_configPath;
    QString m_host;
    int m_port;
    int m_pollInterval;
    bool m_connected;
};
