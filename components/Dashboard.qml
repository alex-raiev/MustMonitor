import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // Battery State of Charge Chart
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            Layout.alignment: Qt.AlignHCenter

            Canvas {
                id: socCanvas
                anchors.centerIn: parent
                width: 200
                height: 200
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0, 0, width, height)
                    var centerX = width / 2
                    var centerY = height / 2
                    var radius = 80
                    var lineWidth = 20
                    
                    ctx.beginPath()
                    ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI)
                    ctx.lineWidth = lineWidth
                    ctx.strokeStyle = "#e0e0e0"
                    ctx.stroke()
                    
                    var progress = dataViewModel.batterySoc / 100
                    ctx.beginPath()
                    ctx.arc(centerX, centerY, radius, -Math.PI / 2, -Math.PI / 2 + progress * 2 * Math.PI)
                    ctx.lineWidth = lineWidth
                    ctx.strokeStyle = "#4CAF50"
                    ctx.stroke()
                }
            }

            Column {
                anchors.centerIn: socCanvas
                spacing: 5
                Label {
                    text: Math.round(dataViewModel.batterySoc) + "%"
                    font.pixelSize: 32
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Label {
                    text: dataViewModel.batterySocName
                    font.pixelSize: 14
                    color: "#666"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        // Load Power %
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            Layout.alignment: Qt.AlignHCenter

            Canvas {
                id: loadCanvas
                anchors.centerIn: parent
                width: 200
                height: 200
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0, 0, width, height)
                    var centerX = width / 2
                    var centerY = height / 2
                    var radius = 80
                    var lineWidth = 20
                    
                    ctx.beginPath()
                    ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI)
                    ctx.lineWidth = lineWidth
                    ctx.strokeStyle = "#e0e0e0"
                    ctx.stroke()
                    
                    var progress = Math.min(dataViewModel.loadPowerPercent / 100, 1)
                    ctx.beginPath()
                    ctx.arc(centerX, centerY, radius, -Math.PI / 2, -Math.PI / 2 + progress * 2 * Math.PI)
                    ctx.lineWidth = lineWidth
                    ctx.strokeStyle = "#2196F3"
                    ctx.stroke()
                }
            }

            Column {
                anchors.centerIn: loadCanvas
                spacing: 5
                Label {
                    text: Math.round(dataViewModel.loadPowerPercent) + "%"
                    font.pixelSize: 32
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Label {
                    text: dataViewModel.loadPowerPercentName
                    font.pixelSize: 14
                    color: "#666"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        // Invertor Voltage - Invertor Current
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "transparent"
            
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "#ADD8E6"
            }
            
            Row {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 40
                
                Column {
                    width: 150
                    spacing: 5
                    Label {
                        text: dataViewModel.invertorVoltage.toFixed(1) + " V"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    Label {
                        text: dataViewModel.invertorVoltageName
                        font.pixelSize: 12
                        color: "#666"
                    }
                }
                
                Column {
                    width: 150
                    spacing: 5
                    Label {
                        text: dataViewModel.invertorCurrent.toFixed(1) + " A"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    Label {
                        text: dataViewModel.invertorCurrentName
                        font.pixelSize: 12
                        color: "#666"
                    }
                }
            }
        }

        // Grid Voltage - Grid Current
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "transparent"
            
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "#ADD8E6"
            }
            
            Row {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 40
                
                Column {
                    width: 150
                    spacing: 5
                    Label {
                        text: dataViewModel.gridVoltage.toFixed(1) + " V"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    Label {
                        text: dataViewModel.gridVoltageName
                        font.pixelSize: 12
                        color: "#666"
                    }
                }
                
                Column {
                    width: 150
                    spacing: 5
                    Label {
                        text: dataViewModel.gridCurrent.toFixed(1) + " A"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    Label {
                        text: dataViewModel.gridCurrentName
                        font.pixelSize: 12
                        color: "#666"
                    }
                }
            }
        }

        // Grid Frequency - Invertor Frequency
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "transparent"
            
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "#ADD8E6"
            }
            
            Row {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 40
                
                Column {
                    width: 150
                    spacing: 5
                    Label {
                        text: dataViewModel.gridFrequency.toFixed(1) + " Hz"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    Label {
                        text: dataViewModel.gridFrequencyName
                        font.pixelSize: 12
                        color: "#666"
                    }
                }
                
                Column {
                    width: 150
                    spacing: 5
                    Label {
                        text: dataViewModel.invertorFrequency.toFixed(1) + " Hz"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    Label {
                        text: dataViewModel.invertorFrequencyName
                        font.pixelSize: 12
                        color: "#666"
                    }
                }
            }
        }
    }

    Connections {
        target: dataViewModel
        function onDataChanged() {
            socCanvas.requestPaint()
            loadCanvas.requestPaint()
        }
    }
}
