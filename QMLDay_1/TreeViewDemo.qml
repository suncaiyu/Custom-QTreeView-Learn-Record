import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import CreateModel 1.0
Window {
    id :win
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    CreateModel {
        id :cm;
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 8
        TreeView {
            id :tv
            width : parent.width
            height : parent.height - 40
            TableViewColumn {
                title: "ID"
                role: "id"
                width: 300
            }
            TableViewColumn {
                title: "API"
                role: "api"
                width: 300
            }
            TableViewColumn {
                title: "DUR"
                role: "dur"
                width: 300
            }
        }
        TextButton {
            id : tb
            width : parent.width
            height : 40
            onClicked: {
                cm.InitModel()
                tv.model = cm.model
            }
        }
    }
}
