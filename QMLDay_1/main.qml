import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls
import QtQuick.Layouts
import MyFunction 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    MyFunction {
        id : mf
    }

    TextButton {
        width: 60
        height:25
        onClicked: {
            mf.callbackFunc();
        }
    }
}
