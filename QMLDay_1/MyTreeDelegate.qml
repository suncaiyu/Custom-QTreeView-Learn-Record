import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Material

T.TreeViewDelegate {
    id: control
    indentation: indicator ? indicator.width : 12
    implicitWidth: /*leftMargin + __contentIndent + implicitContentWidth + rightPadding + rightMargin*/(treeView.width)  / 4
    implicitHeight: Math.max(implicitBackgroundHeight, implicitContentHeight, implicitIndicatorHeight)
    leftMargin: 16
    rightMargin: 16
    spacing: 14

    topPadding: 0
    bottomPadding: 0
    //    leftPadding: 1
    rightPadding: 0
    //    topPadding: contentItem ? (height - contentItem.implicitHeight) / 2 : 0
    leftPadding: !mirrored ? __contentIndent : width - leftMargin - __contentIndent - implicitContentWidth

    highlighted: control.selected || control.current
                 || ((control.treeView.selectionBehavior === TableView.SelectRows
                      || control.treeView.selectionBehavior === TableView.SelectionDisabled)
                     && control.row === control.treeView.currentRow)

    required property int row
    required property var model
    readonly property real __contentIndent: !isTreeNode ? 0 : (depth * indentation) + (indicator ? indicator.width + spacing : 0)

    indicator: Item {
        readonly property real __indicatorIndent: control.leftMargin + (control.depth * control.indentation)
        x: !control.mirrored ? __indicatorIndent : control.width - __indicatorIndent - width
        y: (control.height - height) / 2
        implicitWidth: Math.max(arrow.implicitWidth, 20)
        implicitHeight: control.Material.buttonHeight

        property ColorImage arrow : ColorImage {
            parent: control.indicator
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            rotation:  control.expanded ? 90 : (control.mirrored ? 180 : 0)
            source: "qrc:/qt-project.org/imports/QtQuick/Controls/Material/images/arrow-indicator.png"
            color: control.palette.windowText
            defaultColor: "#353637"
        }
    }

    background: Rectangle {
        implicitHeight: control.Material.buttonHeight
        //        color: control.highlighted
        //               ? control.palette.highlight
        //               : (control.treeView.alternatingRows && control.row % 2 !== 0
        //               ? control.palette.alternateBase : control.palette.base)

        color: {
            if (treeView.selectedRow === row) {
                treeView.selectedColor;
            } else if (treeView.hoverRow === row) {
                Qt.lighter(treeView.selectedColor, 1.15)
            } else {
                control.highlighted
                        ? control.palette.highlight
                        : (control.treeView.alternatingRows && control.row % 2 !== 0
                           ? control.palette.alternateBase : control.palette.base)
            }
        }
    }

    contentItem: Rectangle {
        color: {
            if (treeView.selectedRow === row) {
                treeView.selectedColor;
            } else if (treeView.hoverRow === row) {
                Qt.lighter(treeView.selectedColor, 1.15)
            } else {
                control.highlighted
                        ? control.palette.highlight
                        : (control.treeView.alternatingRows && control.row % 2 !== 0
                           ? control.palette.alternateBase : control.palette.base)
            }
        }
        Text {
            anchors.fill: parent
            text: {
                control.model.display
            }
            verticalAlignment: Text.AlignVCenter 	//垂直居中，控件必须有height才可以使用
            horizontalAlignment: Text.AlignHCenter 	//水平居中，控件必须有width才可以使用
        }
        Button{
            text:"show"
            visible:
                if (column === 3) {
                    true
                } else {
                    false
                }
            anchors.fill: parent
        }
    }
    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            treeView.hoverRow = row;
        }
        onClicked: {
            treeView.selectedRow = row;
//            let ss =  mapToItem(treeView, x, y);
//            console.log(ss);
        }
        onDoubleClicked: {
            if (treeView.isExpanded(row)) {
                treeView.collapse(row);
            } else {
                treeView.expand(row);
            }
        }
    }
}
