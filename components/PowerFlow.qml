import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Item {
    id: root
    
    property int workState: 0
    property int arrowFlag: 0
    property int batteryChargingState: 0
    property int chargerWorkState: 0
    property int mpptState: 0
    property int inverterRelayState: 0
    property int gridRelayState: 0
    property int loadRelayState: 0
    property int dcRelayState: 0
    
    readonly property int connectionOffset: 8
    
    readonly property bool gridToInverter: (arrowFlag & 0x108) !== 0
    readonly property bool inverterToBattery: batteryChargingState > 0
    readonly property bool batteryToInverter: (arrowFlag & 0x44) !== 0 && !inverterToBattery
    readonly property bool pvToInverter: (arrowFlag & 0x11) !== 0
    readonly property bool inverterToLoad: (arrowFlag & 0x22) !== 0
    
    onArrowFlagChanged: console.log("Arrow Flag:", arrowFlag, "Binary:", arrowFlag.toString(2),
                                     "Grid:", gridToInverter, "Battery:", batteryToInverter,
                                     "PV:", pvToInverter, "Load:", inverterToLoad)
    
    // Reusable component box
    component PowerBox: Rectangle {
        property string label: ""
        property string iconText: ""
        property color boxColor: "#16a185"
        property color titleColor: "#1abc9c"
        
        width: 100
        height: 100
        radius: 6
        color: boxColor
        
        Rectangle {
            id: header
            width: parent.width
            height: 22
            radius: parent.radius
            color: titleColor
            
            Rectangle {
                height: parent.height / 2
                width: parent.width
                color: parent.color
                anchors.top: parent.verticalCenter
            }
            
            Text {
                text: parent.parent.label
                font.pixelSize: 13
                font.bold: true
                color: "white"
                anchors.centerIn: parent
            }
        }
        
        Text {
            text: parent.iconText
            font.pixelSize: 48
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 10
        }
    }
    
    // Animated connection line with moving balls
    component PowerConnection: Item {
        id: connection
        property bool active: false
        property color lineColor: "#4789d0"
        property int ballCount: 3
        property bool reverse: false
        property bool isVertical: Math.abs(height) > Math.abs(width)
        
        Rectangle {
            width: connection.isVertical ? 4 : parent.width
            height: connection.isVertical ? parent.height : 4
            color: connection.active ? connection.lineColor : "#E0E0E0"
            anchors.centerIn: parent
            radius: 2
        }
        
        Repeater {
            model: connection.active ? connection.ballCount : 0
            
            Canvas {
                id: pacman
                width: 18
                height: 18
                property real mouthAngle: 0
                
                SequentialAnimation on x {
                    running: connection.active && !connection.isVertical
                    loops: Animation.Infinite
                    NumberAnimation {
                        from: connection.reverse ? connection.width - 18 : 0
                        to: connection.reverse ? 0 : connection.width - 18
                        duration: 2000
                        easing.type: Easing.Linear
                    }
                    PauseAnimation { duration: index * 666 }
                }
                
                SequentialAnimation on y {
                    running: connection.active && connection.isVertical
                    loops: Animation.Infinite
                    NumberAnimation {
                        from: connection.reverse ? connection.height - 18 : 0
                        to: connection.reverse ? 0 : connection.height - 18
                        duration: 2000
                        easing.type: Easing.Linear
                    }
                    PauseAnimation { duration: index * 666 }
                }
                
                SequentialAnimation on mouthAngle {
                    running: connection.active
                    loops: Animation.Infinite
                    NumberAnimation { from: 0; to: 45; duration: 200 }
                    NumberAnimation { from: 45; to: 0; duration: 200 }
                }
                
                x: connection.isVertical ? -5 : (connection.reverse ? connection.width - 18 : 0)
                y: connection.isVertical ? (connection.reverse ? connection.height - 18 : 0) : -5
                
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0, 0, width, height)
                    ctx.fillStyle = connection.lineColor
                    ctx.beginPath()
                    
                    var centerX = 9
                    var centerY = 9
                    var radius = 8
                    var angle = mouthAngle * Math.PI / 180
                    
                    var startAngle = angle
                    var endAngle = 2 * Math.PI - angle
                    
                    if (connection.isVertical) {
                        startAngle = connection.reverse ? (Math.PI / 2 + angle) : (3 * Math.PI / 2 + angle)
                        endAngle = connection.reverse ? (3 * Math.PI / 2 - angle) : (Math.PI / 2 - angle)
                    } else {
                        startAngle = connection.reverse ? (Math.PI + angle) : angle
                        endAngle = connection.reverse ? (Math.PI - angle) : (2 * Math.PI - angle)
                    }
                    
                    ctx.arc(centerX, centerY, radius, startAngle, endAngle)
                    ctx.lineTo(centerX, centerY)
                    ctx.fill()
                }
                
                Connections {
                    target: pacman
                    function onMouthAngleChanged() { pacman.requestPaint() }
                }
            }
        }
    }
    
    // Grid box (top right)
    PowerBox {
        id: gridBox
        x: root.width - width - 30
        y: 30
        label: "Grid"
        iconText: "⚡"
        boxColor: "#8B4545"
        titleColor: "#A85858"
    }
    
    // PV box (top left)
    PowerBox {
        id: pvBox
        x: 30
        y: 30
        label: "Solar"
        iconText: "☀"
        boxColor: "#9B7A3A"
        titleColor: "#B8924A"
    }
    
    // Inverter box (center)
    PowerBox {
        id: inverterBox
        x: root.width / 2 - width / 2
        y: root.height / 2 - height / 2
        label: "Inverter"
        iconText: "⚙"
        boxColor: "#5E4570"
        titleColor: "#7D5A8F"
    }
    
    // Battery box (bottom right)
    PowerBox {
        id: batteryBox
        x: root.width - width - 30
        y: root.height - height - 30
        label: "Battery"
        iconText: "🔋"
        boxColor: "#3D6B4A"
        titleColor: "#4F8A5E"
    }
    
    // Load box (bottom left)
    PowerBox {
        id: loadBox
        x: 30
        y: root.height - height - 30
        label: "Load"
        iconText: "🏠"
        boxColor: "#7A4545"
        titleColor: "#9A5858"
    }
    
    // Grid to Inverter - vertical segment
    PowerConnection {
        x: gridBox.x + gridBox.width / 2 - 2
        y: gridBox.y + gridBox.height
        width: 4
        height: (inverterBox.y - gridBox.y - gridBox.height) / 2
        active: root.gridToInverter
        lineColor: "#5A8A5A"
    }
    
    // Grid to Inverter - horizontal segment
    PowerConnection {
        x: Math.min(gridBox.x + gridBox.width / 2, inverterBox.x + inverterBox.width / 2 + root.connectionOffset)
        y: gridBox.y + gridBox.height + (inverterBox.y - gridBox.y - gridBox.height) / 2 - 2
        width: Math.abs(gridBox.x + gridBox.width / 2 - (inverterBox.x + inverterBox.width / 2 + root.connectionOffset))
        height: 4
        active: root.gridToInverter
        lineColor: "#5A8A5A"
        reverse: true
    }
    
    // Grid to Inverter - final vertical segment
    PowerConnection {
        x: inverterBox.x + inverterBox.width / 2 + root.connectionOffset - 2
        y: gridBox.y + gridBox.height + (inverterBox.y - gridBox.y - gridBox.height) / 2
        width: 4
        height: (inverterBox.y - gridBox.y - gridBox.height) / 2
        active: root.gridToInverter
        lineColor: "#5A8A5A"
    }
    
    // PV to Inverter - vertical segment
    PowerConnection {
        x: pvBox.x + pvBox.width / 2 - 2
        y: pvBox.y + pvBox.height
        width: 4
        height: (inverterBox.y - pvBox.y - pvBox.height) / 2
        active: root.pvToInverter
        lineColor: "#B8924A"
    }
    
    // PV to Inverter - horizontal segment
    PowerConnection {
        x: Math.min(pvBox.x + pvBox.width / 2, inverterBox.x + inverterBox.width / 2 - root.connectionOffset)
        y: pvBox.y + pvBox.height + (inverterBox.y - pvBox.y - pvBox.height) / 2 - 2
        width: Math.abs(inverterBox.x + inverterBox.width / 2 - root.connectionOffset - (pvBox.x + pvBox.width / 2))
        height: 4
        active: root.pvToInverter
        lineColor: "#B8924A"
    }
    
    // PV to Inverter - final vertical segment
    PowerConnection {
        x: inverterBox.x + inverterBox.width / 2 - root.connectionOffset - 2
        y: pvBox.y + pvBox.height + (inverterBox.y - pvBox.y - pvBox.height) / 2
        width: 4
        height: (inverterBox.y - pvBox.y - pvBox.height) / 2
        active: root.pvToInverter
        lineColor: "#B8924A"
    }
    
    // Battery to Inverter - first vertical segment (from battery up)
    PowerConnection {
        x: batteryBox.x + batteryBox.width / 2 - 2
        y: batteryBox.y - (batteryBox.y - inverterBox.y - inverterBox.height) / 2
        width: 4
        height: (batteryBox.y - inverterBox.y - inverterBox.height) / 2
        active: root.batteryToInverter || root.inverterToBattery
        lineColor: "#5A8AAA"
        reverse: root.inverterToBattery
    }
    
    // Battery to Inverter - horizontal segment
    PowerConnection {
        x: inverterBox.x + inverterBox.width / 2 + root.connectionOffset
        y: batteryBox.y - (batteryBox.y - inverterBox.y - inverterBox.height) / 2 - 2
        width: batteryBox.x + batteryBox.width / 2 - (inverterBox.x + inverterBox.width / 2 + root.connectionOffset)
        height: 4
        active: root.batteryToInverter || root.inverterToBattery
        lineColor: "#5A8AAA"
        reverse: root.inverterToBattery
    }
    
    // Battery to Inverter - second vertical segment (to inverter)
    PowerConnection {
        x: inverterBox.x + inverterBox.width / 2 + root.connectionOffset - 2
        y: inverterBox.y + inverterBox.height
        width: 4
        height: (batteryBox.y - inverterBox.y - inverterBox.height) / 2
        active: root.batteryToInverter || root.inverterToBattery
        lineColor: "#5A8AAA"
        reverse: root.inverterToBattery
    }
    
    // Inverter to Load - second vertical segment (down to load)
    PowerConnection {
        x: loadBox.x + loadBox.width / 2 - 2
        y: loadBox.y - (loadBox.y - inverterBox.y - inverterBox.height) / 2
        width: 4
        height: (loadBox.y - inverterBox.y - inverterBox.height) / 2
        active: root.inverterToLoad
        lineColor: "#7D5A8F"
        reverse: false
    }
    
    // Inverter to Load - horizontal segment
    PowerConnection {
        x: loadBox.x + loadBox.width / 2
        y: loadBox.y - (loadBox.y - inverterBox.y - inverterBox.height) / 2 - 2
        width: inverterBox.x + inverterBox.width / 2 - root.connectionOffset - (loadBox.x + loadBox.width / 2)
        height: 4
        active: root.inverterToLoad
        lineColor: "#7D5A8F"
        reverse: false
    }
    
    // Inverter to Load - first vertical segment (from inverter down)
    PowerConnection {
        x: inverterBox.x + inverterBox.width / 2 - root.connectionOffset - 2
        y: inverterBox.y + inverterBox.height
        width: 4
        height: (loadBox.y - inverterBox.y - inverterBox.height) / 2
        active: root.inverterToLoad
        lineColor: "#7D5A8F"
        reverse: false
    }
}
