import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Mini_Wireshark 1.0   // ← 정확히 이거

Item {
    id: root
    width: stk.width
    height: stk.height
    visible: false

    property string nicName: ""

    Component.onCompleted: {

    }

    Pcap{
        id :pcap;
    }

    Image{
        id:img_shark
        source: "img/mini.png"

        width: 100
        height: 100

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea{
            id: img_mouse
            anchors.fill: parent
            onPressed: {
                img_shark.scale = 0.9;
                pcap.create_Th(nicName, filter_tf.text);
                filter_tf.clear();

                // 애니메이션 시작
                capture_animation.visible = true;
                capture_animation.isCapturing = true;
            }
            onReleased: {
                img_shark.scale = 1.0;
            }
        }

        Behavior on scale {
            NumberAnimation{
                duration: 80
                easing.type: Easing.InOutQuad
            }
        }
    }

        Image{
        id:img_stop
        source: "img/stop.png"

        width: 100
        height: 100

        anchors.left: img_shark.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea{
            id: img_stop_m
            anchors.fill: parent
            onPressed: {
                img_stop.scale = 0.9;
                pcap.stop_Th();
                // 애니메이션 정지
                capture_animation.isCapturing = false;
                // capture_animation.visible = false;
            }
            onReleased: {
                img_stop.scale = 1.0;
            }
        }

        Behavior on scale {
            NumberAnimation{
                duration: 80
                easing.type: Easing.InOutQuad
            }
        }
    }



    Text{
        id:nic_text
        text: "Capture :: " + nicName
        font.pixelSize: 18

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: img_shark.bottom
        anchors.topMargin: 5
    }

    // 캡처 중 애니메이션
    Row {
        id: capture_animation
        anchors.left: nic_text.right
        anchors.leftMargin: 15
        anchors.verticalCenter: nic_text.verticalCenter
        spacing: 4
        visible: true

        property bool isCapturing: false  // 캡처 상태

        Repeater {
            model: 5
            Rectangle {
                width: 4
                height: 20
                radius: 2
                color: "#4FC3F7"
                
                // 각 막대마다 다른 타이밍으로 애니메이션
                SequentialAnimation on height {
                    loops: Animation.Infinite
                    running: capture_animation.isCapturing
                    
                    // 딜레이 (각 막대마다 다름)
                    PauseAnimation { 
                        duration: index * 150 
                    }
                    
                    // 올라가기
                    NumberAnimation {
                        from: 8
                        to: 24
                        duration: 400
                        easing.type: Easing.InOutQuad
                    }
                    
                    // 내려가기
                    NumberAnimation {
                        from: 24
                        to: 8
                        duration: 400
                        easing.type: Easing.InOutQuad
                    }
                    
                    // 잠깐 대기
                    PauseAnimation { 
                        duration: 200 
                    }
                }
            }
        }
    }

    TextField{
        id: filter_tf
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: nic_text.bottom
        anchors.topMargin: 5

        width:root.width - 20
        height: 30;

        placeholderText:
            "tcp   /   udp   /   icmp   /   port ..   /   tcp port 443   /   src host 192.168.1.10   /   dst host 8.8.8.8";
    }

    Image{
        id:img_back
        source: "img/back.png"

        width: 100
        height: 70

        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea{
            id: img_back_m
            anchors.fill: parent
            onPressed: {
                img_back.scale = 0.9;
                stk.pop();
            }
            onReleased: {
                img_back.scale = 1.0;
            }
        }

        Behavior on scale {
            NumberAnimation{
                duration: 80
                easing.type: Easing.InOutQuad
            }
        }
    }

    Row {
        id: header
        height: 30
        spacing: 10

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: filter_tf.bottom
        anchors.topMargin: 10

        Rectangle {
            width: 50; height: 30
            color: "#DDDDDD"
            Text { anchors.centerIn: parent; text: "No." }
        }
        Rectangle {
            width: 150; height: 30
            color: "#DDDDDD"
            Text { anchors.centerIn: parent; text: "Source" }
        }
        Rectangle {
            width: 150; height: 30
            color: "#DDDDDD"
            Text { anchors.centerIn: parent; text: "Destination" }
        }
        Rectangle {
            width: 80; height: 30
            color: "#DDDDDD"
            Text { anchors.centerIn: parent; text: "Protocol" }
        }
        Rectangle {
            width: 60; height: 30
            color: "#DDDDDD"
            Text { anchors.centerIn: parent; text: "Length" }
        }
        Rectangle {
            width: 300; height: 30
            color: "#DDDDDD"
            Text { anchors.centerIn: parent; text: "Info" }
        }
    }





    ListView{
        id: l_view
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: header.bottom
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        spacing: 5
        clip: true  // 영역 밖 항목 잘라내기

        model: pcap.md

        delegate: Rectangle{
            width: l_view.width - 20
            height: 30
            color: "#FAFAFA"
            border.color: "#404040"
            border.width: 1
            radius: 5


            Row {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 10

                Text { text: model.number;        width: 50 }    // No
                Text { text: model.source;       width: 150 }   // Source
                Text { text: model.destination;       width: 150 }   // Destination
                Text { text: model.protocol;     width: 80 }    // Protocol
                Text { text: model.length;       width: 60 }    // Length
                Text { text: model.info;      width: 300 }   // Info
            }
        }
    }

}
