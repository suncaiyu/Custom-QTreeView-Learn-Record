import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls/* 1.4*/
import CreateModel 1.0
import QtQuick

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
//        // QtQuick.Controls 1.4 版本treeView  Qt低于6.3
//        TreeView {
//            id :tv
//            width : parent.width
//            height : parent.height - 40
//            itemDelegate: Rectangle {
//                color:"#44ef337d"
//            }

//            TableViewColumn {
//                title: "ID"
//                role: "id"
//                width: 300
//            }
//            TableViewColumn {
//                title: "API"
//                role: "api"
//                width: 300
//            }
//            TableViewColumn {
//                title: "DUR"
//                role: "dur"
//                width: 300
//            }
//        }

        TreeView {
            width : parent.width
            height : parent.height - 40
            id : tv
            delegate: MyTreeViewDelegate{}
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
