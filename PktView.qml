import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Mini_Wireshark 1.0

Item {
    id: root
    anchors.fill: parent
    visible: false

    property int dump_idx: 0
    property Pcap pcap

    Rectangle {
        anchors.fill: parent
        color: "#252525"
    }

    Rectangle {
        id: toolbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 54
        color: "#303030"
        border.color: "#484848"
        border.width: 1

        Label {
            anchors.left: parent.left
            anchors.leftMargin: 14
            anchors.verticalCenter: parent.verticalCenter
            text: "Packet details  #" + dump_idx
            color: "white"
            font.pixelSize: 18
            font.bold: true
        }

        Rectangle {
            id: backButton
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.verticalCenter: parent.verticalCenter
            width: 40
            height: 40
            radius: 6
            color: backMouse.pressed ? "#474747" : "#383838"
            border.color: "#777777"

            Image {
                anchors.fill: parent
                anchors.margins: 5
                source: "img/back_new.svg"
                fillMode: Image.PreserveAspectFit
            }

            MouseArea {
                id: backMouse
                anchors.fill: parent
                onClicked: stk.pop()
            }
        }
    }

    SplitView {
        id: detailSplit
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 10
        orientation: Qt.Vertical

        handle: Rectangle {
            implicitHeight: 6
            color: SplitHandle.pressed ? "#6EADE0"
                                       : SplitHandle.hovered ? "#5A5A5A" : "#424242"
        }

        Rectangle {
            id: treePanel
            SplitView.minimumHeight: 150
            SplitView.preferredHeight: detailSplit.height * 0.56
            color: "#2B2B2B"
            border.color: "#4A4A4A"
            radius: 5
            clip: true

            TreeView {
                id: treeView
                anchors.fill: parent
                anchors.margins: 2
                clip: true
                model: pcap.tree_md

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }

                ScrollBar.horizontal: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }

                delegate: TreeViewDelegate {
                    id: treeDelegate
                    width: treeView.width
                    implicitHeight: 30
                    height: 30
                    rightPadding: 8
                    spacing: 4
                    font.pixelSize: 14
                    palette.text: "#E8E8E8"
                    palette.windowText: "#D8D8D8"
                    palette.highlightedText: "white"

                    indicator: Item {
                        readonly property real indicatorIndent:
                            treeDelegate.leftMargin + treeDelegate.depth * treeDelegate.indentation
                        x: treeDelegate.mirrored
                           ? treeDelegate.width - indicatorIndent - width
                           : indicatorIndent
                        y: (treeDelegate.height - height) / 2
                        implicitWidth: 20
                        implicitHeight: 30

                        Text {
                            anchors.centerIn: parent
                            text: treeDelegate.hasChildren
                                  ? (treeDelegate.expanded ? "\u25BC" : "\u25B6") : ""
                            color: "#E4E4E4"
                            font.pixelSize: 11
                        }

                        MouseArea {
                            anchors.fill: parent
                            enabled: treeDelegate.hasChildren
                            cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                            onClicked: treeView.toggleExpanded(treeDelegate.row)
                        }
                    }

                    contentItem: Text {
                        text: model.display
                        color: "#E8E8E8"
                        font.family: "monospace"
                        font.pixelSize: 14
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    background: Rectangle {
                        color: treeDelegate.down ? "#31526C"
                                                 : treeDelegate.hovered ? "#374957" : "#2B2B2B"
                        border.color: "#383838"
                        border.width: 1
                    }
                }
            }
        }

        Rectangle {
            id: hexPanel
            SplitView.minimumHeight: 130
            SplitView.fillHeight: true
            color: "#1E1E1E"
            border.color: "#4A4A4A"
            radius: 5
            clip: true

            ScrollView {
                anchors.fill: parent
                anchors.margins: 1
                clip: true

                ScrollBar.vertical.policy: ScrollBar.AsNeeded
                ScrollBar.horizontal.policy: ScrollBar.AsNeeded

                TextArea {
                    id: hexArea
                    text: pcap.start_hax(dump_idx)
                    color: "#D8D8D8"
                    font.family: "monospace"
                    font.pixelSize: 14
                    padding: 8
                    wrapMode: Text.NoWrap
                    readOnly: true
                    selectByMouse: true

                    background: Rectangle {
                        color: "#1E1E1E"
                    }
                }
            }
        }
    }
}
