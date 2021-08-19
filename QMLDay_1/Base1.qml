import QtQuick 2.0
Item {
    id: root

    Rectangle {
        id: rect1
        width: 76; height:96
        x: 12; y: 12;
        color: "lightsteelblue"
    }

    Rectangle {
        id: rect2
        x: 112; y : 12
        width:76; height:96
        border.color: "lightsteelblue"
        border.width: 4
        radius: 8
    }

    Rectangle {
        id: rect3
        x:192; y : 12
        width: 76; height : 96
        gradient: Gradient{
            GradientStop{position: 0.0; color:"lightsteelblue"}
            GradientStop{position: 1.0; color:"slategray"}
        }
        border.color: "gray"
    }

    Text {
        id: text1
        text: qsTr("The Quick Brown Fox")
        color: "#303030"
        font.family: "Ubuntu"
        font.pixelSize: 28
        x: 0; y:108
    }
}


