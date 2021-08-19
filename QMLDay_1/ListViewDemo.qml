import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id :win
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    ListView {
        id : lv
        anchors.fill:parent
        anchors.margins: 10
        clip: true
        model:100
        delegate: mydelegate
        highlight: Rectangle {
            width: lv.width
            height: 40
            color: "lightsteelblue"
            radius: 5
            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }
        }
        focus: true
    }

    Component{
        id : mydelegate
        //        TextButton {
        //            id : tb
        //            mText: index
        //            width : parent.width
        //            height:30
        //            Connections{
        //                target: win
        //                onMCurIndex:{
        //                    if (index == idx) {
        //                        tb.mColor = "red"
        //                    } else {
        //                        tb.mColor = "hotpink"
        //                    }
        //                }
        //            }
        //        }
        Item {
            width : parent.width
            height : 30
            Text{
                anchors.centerIn: parent
                text: index
            }
        }
    }
}
