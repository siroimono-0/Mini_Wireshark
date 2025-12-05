import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Mini_Wireshark 1.0

Item {
    width: stk.width
    height: stk.height
    visible: false

    property int dump_idx: 0;
    property Pcap pcap;

    Component.onCompleted: {
        // pcap.start_tree_md();
    }

    Rectangle{
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.verticalCenter

        color: "#2B2B2B"  // 어두운 배경색 (Wireshark 스타일)
        border.color: "#404040"
        border.width: 1
        radius: 5

        TreeView{
            id: tView;
            anchors.fill: parent
            anchors.margins: 1

            clip: true

            model: pcap.tree_md;

            delegate: TreeViewDelegate{
                width: tView.width

                contentItem: RowLayout {
                    spacing: 4

                    Text {
                        text: model.display
                        color: "white"
                        font.pixelSize: 14
                        font.family: "monospace"
                        width: parent.width
                    }
                }
            }
        }
    }

    Image {
        id: img_back
        source: "img/pkt_back.png"

        width: 100
        height: 80

        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea {
            id: img_back_m
            anchors.fill: parent
            onPressed: {
                img_back.scale = 0.9
                stk.pop();
            }
            onReleased: {
                img_back.scale = 1.0
            }
        }

        Behavior on scale {
            NumberAnimation {
                duration: 80
                easing.type: Easing.InOutQuad
            }
        }
    }

    // Hex Dump (하단 절반)
    ScrollView {
        id: scrollView
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.verticalCenter  // 중앙부터 시작
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        clip: true

        ScrollBar.vertical: ScrollBar {
            active: true
            policy: ScrollBar.AlwaysOn

            contentItem: Rectangle {
                implicitWidth: 8
                radius: 4
                color: parent.pressed ? "#606060" : "#808080"
                opacity: parent.hovered ? 1.0 : 0.7
            }

            background: Rectangle {
                implicitWidth: 10
                color: "#E0E0E0"
                radius: 5
            }
        }

        ScrollBar.horizontal: ScrollBar {
            active: true
            policy: ScrollBar.AsNeeded

            contentItem: Rectangle {
                implicitHeight: 8
                radius: 4
                color: parent.pressed ? "#606060" : "#808080"
                opacity: parent.hovered ? 1.0 : 0.7
            }

            background: Rectangle {
                implicitHeight: 10
                color: "#E0E0E0"
                radius: 5
            }
        }

        TextArea {
            id: tArea
            font.family: "monospace"
            font.pixelSize: 14
            wrapMode: Text.NoWrap
            readOnly: true

            text: pcap.start_hax(dump_idx)

            background: Rectangle {
                color: "#1E1E1E"  // 어두운 배경
                border.color: "#404040"
                border.width: 1
                radius: 5
            }

            color: "#D4D4D4"  // 밝은 텍스트 색상
        }
    }
}


