import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    TextButton {
        anchors.centerIn: parent
        mText: "Button"
        width: 100
        height:30
        onClicked: {
            console.log("scy", "xxx")
        }
    }
}
