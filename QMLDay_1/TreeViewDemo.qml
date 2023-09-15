import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick;
import CreateModel 1.0
import QtQuick.Controls
import QtQuick.Templates as T;
Window {
    id :win
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    CreateModel {
        id :cm;
    }

    //    Column {
    //        anchors.fill: parent
    //        anchors.margins: 20
    //        spacing: 8
    //        TreeView {
    //            id :tv
    //            width : parent.width
    //            height : parent.height - 40
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
    //        TextButton {
    //            id : tb
    //            width : parent.width
    //            height : 40
    //            onClicked: {
    //                cm.InitModel()
    //                tv.model = cm.model
    //            }
    //        }
    //    }

    TreeView {
        id:treeview
        anchors.fill: parent
        width:parent.width
        height:parent.height - 30
        clip:true

        model:cm.model
//        model:
//            ListModel {
//                id: fruitModel

//                ListElement {
//                    display: "Apple"
//                    cost: 2.45
//                }
//                ListElement {
//                    display: "Orange"
//                    cost: 3.25
//                }
//                ListElement {
//                    display: "Banana"
//                    cost: 1.95
//                }
//            }

//                delegate:TreeViewDelegate{}
        delegate: MyTreeDelegate{}

        property int hoverRow: -1
        property int selectedRow: -1
        property color selectedColor: "#46b2b5"
        onSelectedRowChanged:{
            seq.running
        }


        Behavior on selectedRow {
            //            NumberAnimation{
            //                duration:2000
            //            }
            SequentialAnimation {
                id :seq
                ColorAnimation{
                    target: treeview
                    property:"selectedColor"
                    from: "#46b2b5"
                    to: "white"
                    duration: 200
                }

                ColorAnimation{
                    target: treeview
                    property:"selectedColor"
                    from: "white"
                    to: "#46b2b5"
                    duration: 200
                }
            }
        }
    }
}
