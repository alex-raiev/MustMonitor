import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 400
    height: 680
    title: "Must Monitor"

    TabBar {
        id: tabBar
        width: parent.width
        
        TabButton { text: "Dashboard" }
        TabButton { text: "Power Flow" }
        TabButton { text: "Charts" }
        TabButton { text: "Logs" }
        TabButton { text: "Settings" }
    }

    StackLayout {
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        currentIndex: tabBar.currentIndex

        Dashboard {}
        PowerFlow {
            workState: dataViewModel.workState
            arrowFlag: dataViewModel.arrowFlag
            batteryChargingState: dataViewModel.batteryChargingState
            chargerWorkState: dataViewModel.chargerWorkState
            mpptState: dataViewModel.mpptState
            inverterRelayState: dataViewModel.inverterRelayState
            gridRelayState: dataViewModel.gridRelayState
            loadRelayState: dataViewModel.loadRelayState
            dcRelayState: dataViewModel.dcRelayState
        }
        Charts {}
        Logs {}
        Item {
            anchors.fill: parent
            Settings {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 100
                anchors.leftMargin: 15
                anchors.rightMargin: 15
            }
        }
    }
}
