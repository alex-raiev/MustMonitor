#pragma once
#include <QObject>
#include <QString>

class DataViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(double batterySoc READ batterySoc NOTIFY dataChanged)
    Q_PROPERTY(double loadPowerPercent READ loadPowerPercent NOTIFY dataChanged)
    Q_PROPERTY(double invertorVoltage READ invertorVoltage NOTIFY dataChanged)
    Q_PROPERTY(double invertorCurrent READ invertorCurrent NOTIFY dataChanged)
    Q_PROPERTY(double gridVoltage READ gridVoltage NOTIFY dataChanged)
    Q_PROPERTY(double gridCurrent READ gridCurrent NOTIFY dataChanged)
    Q_PROPERTY(double gridFrequency READ gridFrequency NOTIFY dataChanged)
    Q_PROPERTY(double invertorFrequency READ invertorFrequency NOTIFY dataChanged)
    Q_PROPERTY(double batteryTemperature READ batteryTemperature NOTIFY dataChanged)
    Q_PROPERTY(double loadPowerKw READ loadPowerKw NOTIFY dataChanged)
    Q_PROPERTY(double inverterPowerKva READ inverterPowerKva NOTIFY dataChanged)
    Q_PROPERTY(QString batterySocName READ batterySocName NOTIFY namesChanged)
    Q_PROPERTY(QString loadPowerPercentName READ loadPowerPercentName NOTIFY namesChanged)
    Q_PROPERTY(QString invertorVoltageName READ invertorVoltageName NOTIFY namesChanged)
    Q_PROPERTY(QString invertorCurrentName READ invertorCurrentName NOTIFY namesChanged)
    Q_PROPERTY(QString gridVoltageName READ gridVoltageName NOTIFY namesChanged)
    Q_PROPERTY(QString gridCurrentName READ gridCurrentName NOTIFY namesChanged)
    Q_PROPERTY(QString gridFrequencyName READ gridFrequencyName NOTIFY namesChanged)
    Q_PROPERTY(QString invertorFrequencyName READ invertorFrequencyName NOTIFY namesChanged)
    Q_PROPERTY(QString batteryTemperatureName READ batteryTemperatureName NOTIFY namesChanged)
    Q_PROPERTY(QString loadPowerKwName READ loadPowerKwName NOTIFY namesChanged)
    Q_PROPERTY(QString inverterPowerKvaName READ inverterPowerKvaName NOTIFY namesChanged)
    
public:
    explicit DataViewModel(QObject *parent = nullptr);
    
    double batterySoc() const { return m_batterySoc; }
    double loadPowerPercent() const { return m_loadPowerPercent; }
    double invertorVoltage() const { return m_invertorVoltage; }
    double invertorCurrent() const { return m_invertorCurrent; }
    double gridVoltage() const { return m_gridVoltage; }
    double gridCurrent() const { return m_gridCurrent; }
    double gridFrequency() const { return m_gridFrequency; }
    double invertorFrequency() const { return m_invertorFrequency; }
    double batteryTemperature() const { return m_batteryTemperature; }
    double loadPowerKw() const { return m_loadPowerKw; }
    double inverterPowerKva() const { return m_inverterPowerKva; }
    
    QString batterySocName() const { return m_batterySocName; }
    QString loadPowerPercentName() const { return m_loadPowerPercentName; }
    QString invertorVoltageName() const { return m_invertorVoltageName; }
    QString invertorCurrentName() const { return m_invertorCurrentName; }
    QString gridVoltageName() const { return m_gridVoltageName; }
    QString gridCurrentName() const { return m_gridCurrentName; }
    QString gridFrequencyName() const { return m_gridFrequencyName; }
    QString invertorFrequencyName() const { return m_invertorFrequencyName; }
    QString batteryTemperatureName() const { return m_batteryTemperatureName; }
    QString loadPowerKwName() const { return m_loadPowerKwName; }
    QString inverterPowerKvaName() const { return m_inverterPowerKvaName; }
    
public slots:
    void updateData(double batterySoc, double loadPowerPercent, double invertorVoltage,
                    double invertorCurrent, double gridVoltage, double gridCurrent,
                    double gridFrequency, double invertorFrequency, double batteryTemperature,
                    double loadPowerKw, double inverterPowerKva);
    void setRegisterNames(const QString& batterySoc, const QString& loadPowerPercent,
                         const QString& invertorVoltage, const QString& invertorCurrent,
                         const QString& gridVoltage, const QString& gridCurrent,
                         const QString& gridFrequency, const QString& invertorFrequency,
                         const QString& batteryTemperature, const QString& loadPowerKw,
                         const QString& inverterPowerKva);
    
signals:
    void dataChanged();
    void namesChanged();
    
private:
    double m_batterySoc = 0.0;
    double m_loadPowerPercent = 0.0;
    double m_invertorVoltage = 0.0;
    double m_invertorCurrent = 0.0;
    double m_gridVoltage = 0.0;
    double m_gridCurrent = 0.0;
    double m_gridFrequency = 0.0;
    double m_invertorFrequency = 0.0;
    double m_batteryTemperature = 0.0;
    double m_loadPowerKw = 0.0;
    double m_inverterPowerKva = 0.0;
    QString m_batterySocName;
    QString m_loadPowerPercentName;
    QString m_invertorVoltageName;
    QString m_invertorCurrentName;
    QString m_gridVoltageName;
    QString m_gridCurrentName;
    QString m_gridFrequencyName;
    QString m_invertorFrequencyName;
    QString m_batteryTemperatureName;
    QString m_loadPowerKwName;
    QString m_inverterPowerKvaName;
};
