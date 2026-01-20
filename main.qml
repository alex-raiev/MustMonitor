import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 400
    height: 800
    title: "Must Monitor"

    TabBar {
        id: tabBar
        width: parent.width
        
        TabButton { text: "Dashboard" }
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
        Charts {}
        Logs {}
        Settings {}
    }
}
