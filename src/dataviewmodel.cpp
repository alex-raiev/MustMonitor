#include "dataviewmodel.h"
#include <QDebug>

DataViewModel::DataViewModel(QObject *parent)
    : QObject(parent)
{
}

void DataViewModel::updateData(double batterySoc, double loadPowerPercent, double invertorVoltage,
                               double invertorCurrent, double gridVoltage, double gridCurrent,
                               double gridFrequency, double invertorFrequency, double batteryTemperature,
                               double loadPowerKw, double inverterPowerKva) {
    m_batterySoc = batterySoc;
    m_loadPowerPercent = loadPowerPercent;
    m_invertorVoltage = invertorVoltage;
    m_invertorCurrent = invertorCurrent;
    m_gridVoltage = gridVoltage;
    m_gridCurrent = gridCurrent;
    m_gridFrequency = gridFrequency;
    m_invertorFrequency = invertorFrequency;
    m_batteryTemperature = batteryTemperature;
    m_loadPowerKw = loadPowerKw;
    m_inverterPowerKva = inverterPowerKva;
    
    emit dataChanged();
}

void DataViewModel::setRegisterNames(const QString& batterySoc, const QString& loadPowerPercent,
                                     const QString& invertorVoltage, const QString& invertorCurrent,
                                     const QString& gridVoltage, const QString& gridCurrent,
                                     const QString& gridFrequency, const QString& invertorFrequency,
                                     const QString& batteryTemperature, const QString& loadPowerKw,
                                     const QString& inverterPowerKva) {
    m_batterySocName = batterySoc;
    m_loadPowerPercentName = loadPowerPercent;
    m_invertorVoltageName = invertorVoltage;
    m_invertorCurrentName = invertorCurrent;
    m_gridVoltageName = gridVoltage;
    m_gridCurrentName = gridCurrent;
    m_gridFrequencyName = gridFrequency;
    m_invertorFrequencyName = invertorFrequency;
    m_batteryTemperatureName = batteryTemperature;
    m_loadPowerKwName = loadPowerKw;
    m_inverterPowerKvaName = inverterPowerKva;
    emit namesChanged();
}

void DataViewModel::updateState(int workState, int arrowFlag, int batteryChargingState, int chargerWorkState,
                               int mpptState, int inverterRelayState, int gridRelayState, int loadRelayState, int dcRelayState) {
    m_workState = workState;
    m_arrowFlag = arrowFlag;
    m_batteryChargingState = batteryChargingState;
    m_chargerWorkState = chargerWorkState;
    m_mpptState = mpptState;
    m_inverterRelayState = inverterRelayState;
    m_gridRelayState = gridRelayState;
    m_loadRelayState = loadRelayState;
    m_dcRelayState = dcRelayState;
    emit stateChanged();
}
