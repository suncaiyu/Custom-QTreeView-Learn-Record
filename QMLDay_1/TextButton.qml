import QtQuick 2.0

Rectangle {
    id : root
    property color mColor: "hotpink"
    property string mText: "Text"
    signal clicked

    Rectangle {
        id:mybtn
        anchors.fill: parent
        color: mColor
        border.color: Qt.lighter(color)
        border.width: 1
        radius: 8
        state: "nothover"
        focus: true

        Text {
            id: label
            text: mText
            anchors.centerIn: parent
        }

        MouseArea{
            hoverEnabled: true
            anchors.fill:parent
            onClicked: {
                root.clicked()
            }
            onEntered:mybtn.state = "hover"
            onExited:mybtn.state = "nothover"
        }

        states: [
            State {
                name: "hover"
                PropertyChanges {
                    target: mybtn
                    color: Qt.lighter(mColor)
                }
            },
            State {
                name: "nothover"
                PropertyChanges {
                    target: mybtn
                    color:mColor
                }
            }
        ]

        transitions: [
            Transition {
                from: "*"
                to: "*"
                ColorAnimation {
                    target: mybtn
                    duration: 200
                }

            }
        ]
    }
}
