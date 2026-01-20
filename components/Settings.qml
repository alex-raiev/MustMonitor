import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        width: 400

        Label {
            text: "Modbus Connection Settings"
            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#ccc"
        }

        // Connection Status
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Status:"
                font.pixelSize: 16
                Layout.preferredWidth: 150
            }
            Rectangle {
                width: 12
                height: 12
                radius: 6
                color: settingsManager.connected ? "#4CAF50" : "#F44336"
            }
            Label {
                text: settingsManager.connected ? "Connected" : "Disconnected"
                font.pixelSize: 16
                color: settingsManager.connected ? "#4CAF50" : "#F44336"
            }
        }

        // Host
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Host:"
                font.pixelSize: 16
                Layout.preferredWidth: 150
            }
            TextField {
                id: hostField
                text: settingsManager.host
                font.pixelSize: 14
                Layout.fillWidth: true
                onTextChanged: settingsManager.host = text
            }
        }

        // Port
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Port:"
                font.pixelSize: 16
                Layout.preferredWidth: 150
            }
            TextField {
                id: portField
                text: settingsManager.port.toString()
                font.pixelSize: 14
                Layout.preferredWidth: 100
                validator: IntValidator { bottom: 1; top: 65535 }
                onTextChanged: settingsManager.port = parseInt(text) || 502
            }
        }

        // Poll Interval
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Poll Interval (ms):"
                font.pixelSize: 16
                Layout.preferredWidth: 150
            }
            TextField {
                id: pollField
                text: settingsManager.pollInterval.toString()
                font.pixelSize: 14
                Layout.preferredWidth: 100
                validator: IntValidator { bottom: 100; top: 10000 }
                onTextChanged: settingsManager.pollInterval = parseInt(text) || 1000
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#ccc"
        }

        // Buttons
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 10

            Button {
                text: "Save"
                font.pixelSize: 14
                onClicked: {
                    if (settingsManager.saveConfig()) {
                        statusLabel.text = "Settings saved successfully"
                        statusLabel.color = "#4CAF50"
                    } else {
                        statusLabel.text = "Failed to save settings"
                        statusLabel.color = "#F44336"
                    }
                }
            }

            Button {
                text: "Apply & Reconnect"
                font.pixelSize: 14
                onClicked: {
                    settingsManager.saveConfig()
                    settingsManager.applySettings()
                    statusLabel.text = "Reconnecting..."
                    statusLabel.color = "#2196F3"
                }
            }
        }

        Label {
            id: statusLabel
            text: ""
            font.pixelSize: 14
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
