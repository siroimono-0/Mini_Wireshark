import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Mini_Wireshark 1.0

Item {
    id: root
    anchors.fill: parent
    visible: false

    property string selectNic: ""
    readonly property bool compact: width < 1000
    readonly property int pageMargin: width < 820 ? 14 : 24

    NicList {
        id: nic
    }

    Open_List {
        id: open_list
    }

    Component.onCompleted: selectNic = nic.get_FirstNic()

    Rectangle {
        anchors.fill: parent
        color: "#303030"
    }

    FileDialog {
        id: pcapDialog
        title: "Open Pcap File"
        nameFilters: ["PCAP files (*.pcap *.pcapng)", "All files (*)"]

        onAccepted: {
            open_list.add_db(selectedFile)
            stk.push("CaptureView.qml", {
                nicName: root.selectNic,
                pcap_path: selectedFile
            })
        }
    }

    Rectangle {
        id: topBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 62
        color: "#252525"
        border.color: "#474747"
        border.width: 1

        Label {
            anchors.left: parent.left
            anchors.leftMargin: root.pageMargin
            anchors.verticalCenter: parent.verticalCenter
            text: "Mini Wireshark"
            color: "white"
            font.pixelSize: root.compact ? 21 : 24
            font.bold: true
        }

        Button {
            id: openButton
            anchors.right: parent.right
            anchors.rightMargin: root.pageMargin
            anchors.verticalCenter: parent.verticalCenter
            width: root.compact ? 108 : 124
            height: 36
            text: "Open Pcap"

            contentItem: Text {
                text: openButton.text
                color: "#202020"
                font.pixelSize: 14
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: openButton.down ? "#B9DFFF" : "#EAF5FF"
                border.color: "#78BFFF"
                radius: 7
            }

            onClicked: pcapDialog.open()
        }
    }

    Item {
        id: content
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: topBar.bottom
        anchors.bottom: parent.bottom
        anchors.margins: root.pageMargin

        Image {
            id: logo
            visible: !root.compact
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: Math.min(390, content.width * 0.34)
            source: "img/wireshark_it_security_neon_white.svg"
            fillMode: Image.PreserveAspectFit
            opacity: 0.88
        }

        Item {
            id: listColumn
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: root.compact ? parent.right : logo.left
            anchors.rightMargin: root.compact ? 0 : 24

            Rectangle {
                id: recentPanel
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: Math.max(170, Math.min(235, listColumn.height * 0.42))
                color: "#353535"
                border.color: "#626262"
                radius: 8

                Label {
                    id: recentTitle
                    anchors.left: parent.left
                    anchors.leftMargin: 14
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    text: "Recent captures"
                    color: "white"
                    font.pixelSize: 20
                    font.bold: true
                }

                ListView {
                    id: openView
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: recentTitle.bottom
                    anchors.bottom: parent.bottom
                    anchors.margins: 12
                    anchors.topMargin: 8
                    spacing: 5
                    clip: true
                    model: open_list
                    focus: true
                    highlightFollowsCurrentItem: true

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }

                    delegate: Rectangle {
                        width: openView.width
                        height: 36
                        color: ListView.isCurrentItem ? "#D6EBFF" : "#FAFAFA"
                        border.color: ListView.isCurrentItem ? "#2196F3" : "#666666"
                        border.width: ListView.isCurrentItem ? 2 : 1
                        radius: 6

                        Text {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.path
                            color: ListView.isCurrentItem ? "#145D94" : "#202020"
                            font.pixelSize: 14
                            elide: Text.ElideMiddle
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: openView.currentIndex = index
                            onDoubleClicked: {
                                stk.push("CaptureView.qml", {
                                    nicName: root.selectNic,
                                    pcap_path: model.path
                                })
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: capturePanel
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: recentPanel.bottom
                anchors.topMargin: 14
                anchors.bottom: parent.bottom
                color: "#353535"
                border.color: "#626262"
                radius: 8

                Label {
                    id: captureTitle
                    anchors.left: parent.left
                    anchors.leftMargin: 14
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    text: "Capture interfaces"
                    color: "white"
                    font.pixelSize: 20
                    font.bold: true
                }

                ComboBox {
                    id: filterBox
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    anchors.verticalCenter: captureTitle.verticalCenter
                    width: Math.min(180, capturePanel.width * 0.36)
                    height: 34
                    model: ["All", "Wireless", "Local", "Virtual"]

                    onActivated: function(index) {
                        if (index === 0) nic.set_Nic(5)
                        else if (index === 1) nic.set_Nic(2)
                        else if (index === 2) nic.set_Nic(3)
                        else nic.set_Nic(4)

                        root.selectNic = nic.get_FirstNic()
                    }
                }

                ListView {
                    id: nicView
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: captureTitle.bottom
                    anchors.bottom: parent.bottom
                    anchors.margins: 12
                    anchors.topMargin: 8
                    spacing: 5
                    clip: true
                    model: nic
                    focus: true
                    highlightFollowsCurrentItem: true

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }

                    delegate: Rectangle {
                        width: nicView.width
                        height: 36
                        color: ListView.isCurrentItem ? "#D6EBFF" : "#FAFAFA"
                        border.color: ListView.isCurrentItem ? "#2196F3" : "#666666"
                        border.width: ListView.isCurrentItem ? 2 : 1
                        radius: 6

                        Text {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.text
                            color: ListView.isCurrentItem ? "#145D94" : "#202020"
                            font.pixelSize: 14
                            elide: Text.ElideRight
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                nicView.currentIndex = index
                                root.selectNic = model.text
                            }
                            onDoubleClicked: {
                                stk.push("CaptureView.qml", { nicName: model.text })
                            }
                        }
                    }
                }
            }
        }
    }
}
