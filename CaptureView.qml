import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Dialogs
import Mini_Wireshark 1.0

Item {
    id: root
    width: stk.width
    height: stk.height
    visible: false

    property string nicName: ""
    property string pcap_path: ""
    readonly property real toolbarIconSize: Math.max(56, Math.min(78, width / 10))
    readonly property real tableContentWidth: Math.max(560, l_view.width - 20)
    readonly property real noColumnWidth: Math.max(44, tableContentWidth * 0.07)
    readonly property real sourceColumnWidth: Math.max(105, tableContentWidth * 0.18)
    readonly property real destinationColumnWidth: Math.max(105, tableContentWidth * 0.18)
    readonly property real protocolColumnWidth: Math.max(70, tableContentWidth * 0.11)
    readonly property real lengthColumnWidth: Math.max(58, tableContentWidth * 0.09)
    readonly property real infoColumnWidth: Math.max(120,
        tableContentWidth - noColumnWidth - sourceColumnWidth - destinationColumnWidth
        - protocolColumnWidth - lengthColumnWidth - header.spacing * 5)

    Component.onCompleted: {
        if (pcap_path != "") {
            pcap.pcapFile_Read(pcap_path)
        }
    }

    Image {
        id: img_shark

        width: root.toolbarIconSize
        height: root.toolbarIconSize

        source: capture_animation.isCapturing
                ? "img/start_gray.svg"
                : "img/start_.svg"

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea {
            id: img_mouse
            anchors.fill: parent

            enabled: !capture_animation.isCapturing;
            onPressed: {
                img_shark.scale = 0.9
                img_shark_border.scale = 0.9
                pcap.create_Th(nicName, filter_tf.text)
                filter_tf.clear()

                // 애니메이션 시작
                capture_animation.visible = true
                capture_animation.isCapturing = true
            }
            onReleased: {
                img_shark.scale = 1.0
                img_shark_border.scale = 1.0
            }
        }

        Rectangle{
            id:img_shark_border
            color: "transparent"
            border.color: "#FFFFFF"
            border.width: 2
            anchors.fill: parent
            anchors.margins: -2

            layer.enabled: true

            layer.effect: MultiEffect{
                shadowEnabled: true;
                shadowBlur: 1
                shadowColor: "#FFFFFF"
                shadowOpacity: 1
                shadowVerticalOffset: 0
                shadowHorizontalOffset: 0
            }
            Behavior on scale {
                NumberAnimation {
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Behavior on scale {
            NumberAnimation {
                duration: 80
                easing.type: Easing.InOutQuad
            }
        }
    }

    Image {
        id: img_stop

        width: root.toolbarIconSize
        height: root.toolbarIconSize

        source: capture_animation.isCapturing
                ? "img/stop_new.svg"
                : "img/stop_new_gray.svg"


        anchors.left: img_shark.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea {
            id: img_stop_m
            anchors.fill: parent

            enabled: capture_animation.isCapturing;

            onPressed: {
                img_stop.scale = 0.9
                img_stop_border.scale = 0.9
                pcap.stop_Th()
                // 애니메이션 정지
                capture_animation.isCapturing = false
                // capture_animation.visible = false;

            }
            onReleased: {
                img_stop.scale = 1.0
                img_stop_border.scale = 1.0
            }
        }

        Rectangle{
            id: img_stop_border
            color: "transparent"
            border.color: "#FFFFFF"
            border.width: 2
            anchors.fill: parent
            anchors.margins: -2

            layer.enabled: true

            layer.effect: MultiEffect{
                shadowEnabled: true;
                shadowBlur: 1
                shadowColor: "#FFFFFF"
                shadowOpacity: 1
                shadowVerticalOffset: 0
                shadowHorizontalOffset: 0
            }
            Behavior on scale {
                NumberAnimation {
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Behavior on scale {
            NumberAnimation {
                duration: 80
                easing.type: Easing.InOutQuad
            }
        }
    }

    Image {
        id: img_reset
        source: "img/reset_new.svg"

        width: root.toolbarIconSize
        height: root.toolbarIconSize

        anchors.left: img_stop.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea {
            id: img_reset_m
            anchors.fill: parent
            onPressed: {
                img_reset.scale = 0.9
                img_reset_border.scale = 0.9;
                pcap.reset_md()
            }
            onReleased: {
                img_reset.scale = 1.0
                img_reset_border.scale = 1.0;
            }
        }

        Rectangle{
            id:  img_reset_border
            color: "transparent"
            border.color: "#FFFFFF"
            border.width: 2
            anchors.fill: parent
            anchors.margins: -2

            layer.enabled: true

            layer.effect: MultiEffect{
                shadowEnabled: true;
                shadowBlur: 1
                shadowColor: "#FFFFFF"
                shadowOpacity: 1
                shadowVerticalOffset: 0
                shadowHorizontalOffset: 0
            }
            Behavior on scale {
                NumberAnimation {
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Behavior on scale {
            NumberAnimation {
                duration: 80
                easing.type: Easing.InOutQuad
            }
        }
    }

    Image {
        id: img_save
        source: func_save()

        function func_save()
        {
            if(pcap.stop_flag === true && pcap.wk_flag === false)
            {
                return "img/save_new.svg"
            }
            else             {
                return "img/save_new_gray.svg"
            }
        }

        function func_can()
        {
            if(pcap.stop_flag === true && pcap.wk_flag === false)
            {
                return true
            }
            else
            {
                return false
            }
        }

        width: root.toolbarIconSize
        height: root.toolbarIconSize

        anchors.left: img_reset.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea {
            id: img_save_m
            anchors.fill: parent
            enabled: img_save.func_can();

            onPressed: {
                img_save.scale = 0.9
                img_save_border.scale = 0.9
                saveDia.open();
            }
            onReleased: {
                img_save.scale = 1.0
                img_save_border.scale = 1.0
            }
        }

        Rectangle{
            id:   img_save_border
            color: "transparent"
            border.color: "#FFFFFF"
            border.width: 2
            anchors.fill: parent
            anchors.margins: -2

            layer.enabled: true

            layer.effect: MultiEffect{
                shadowEnabled: true;
                shadowBlur: 1
                shadowColor: "#FFFFFF"
                shadowOpacity: 1
                shadowVerticalOffset: 0
                shadowHorizontalOffset: 0
            }
            Behavior on scale {
                NumberAnimation {
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Behavior on scale {
            NumberAnimation {
                duration: 80
                easing.type: Easing.InOutQuad
            }
        }
    }

    FileDialog{
        id: saveDia;
        title: "Save Pcap File";
        fileMode: FileDialog.SaveFile;
        nameFilters: ["PCAP Files (*.pcap)"];
        defaultSuffix: "pcap";

        onAccepted: {
            pcap.save_md(saveDia.selectedFile);
        }
    }

    Text {
        id: nic_text
        text: "Capture :: " + nicName
        font.pixelSize: 18
        color: "white"

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

        property bool isCapturing: false // 캡처 상태

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

    TextField {
        id: filter_tf
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: nic_text.bottom
        anchors.topMargin: 5

        width: root.width - 20
        height: 30

        placeholderTextColor: "gray"
        placeholderText: "tcp   /   udp   /   icmp   /   port ..   /   tcp port 443   /   src host 192.168.1.10   /   dst host 8.8.8.8"
    }

    Image {
        id: img_back
        source: "img/back_new.svg"

        width: root.toolbarIconSize
        height: root.toolbarIconSize

        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea {
            id: img_back_m
            anchors.fill: parent
            onPressed: {
                img_back.scale = 0.9
                img_back_border.scale = 0.9
                pcap.stop_Th();
                pcap.reset_md();
                stk.pop()
            }
            onReleased: {
                img_back.scale = 1.0
                img_back_border.scale = 1.0
            }
        }

        Rectangle{
            id:  img_back_border
            color: "transparent"
            border.color: "#FFFFFF"
            border.width: 2
            anchors.fill: parent
            anchors.margins: -2

            layer.enabled: true

            layer.effect: MultiEffect{
                shadowEnabled: true;
                shadowBlur: 1
                shadowColor: "#FFFFFF"
                shadowOpacity: 1
                shadowVerticalOffset: 0
                shadowHorizontalOffset: 0
            }

            Behavior on scale {
                NumberAnimation {
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Behavior on scale {
            NumberAnimation {
                duration: 80
                easing.type: Easing.InOutQuad
            }
        }
    }

    Row {
        id: header
        height: 30
        spacing: 4

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: filter_tf.bottom
        anchors.topMargin: 10

        Rectangle {
            id: no;

            width: root.noColumnWidth
            height: 30
            border.color: "#DDDDDD"
            border.width: 2

            Behavior on scale {
                NumberAnimation{
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }

            Text {
                anchors.centerIn: parent
                text: "No."
            }

            MouseArea{
                anchors.fill: parent
                onPressed: {
                    no.scale = 0.9;
                    pcap.md.sort_num();
                }
                onReleased: {
                    no.scale = 1.0;
                }
            }

        }
        Rectangle {
            id: source

            width: root.sourceColumnWidth
            height: 30
            border.color: "#DDDDDD"
            border.width: 2

            Behavior on scale {
                NumberAnimation{
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }

            Text {
                anchors.centerIn: parent
                text: "Source"
            }

            MouseArea{
                anchors.fill: parent
                onPressed: {
                    source.scale = 0.9;
                    pcap.md.sort_src();
                }
                onReleased: {
                    source.scale = 1.0;
                }
            }

        }
        Rectangle {
            id: destinaion

            width: root.destinationColumnWidth
            height: 30
            border.color: "#DDDDDD"
            border.width: 2

            Behavior on scale {
                NumberAnimation{
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }

            Text {
                anchors.centerIn: parent
                text: "Destination"
            }

            MouseArea{
                anchors.fill: parent
                onPressed: {
                    destinaion.scale = 0.9;
                    pcap.md.sort_dst();
                }
                onReleased: {
                    destinaion.scale = 1.0;
                }
            }

        }
        Rectangle {
            id: protocol

            width: root.protocolColumnWidth
            height: 30
            border.color: "#DDDDDD"
            border.width: 2

            Behavior on scale {
                NumberAnimation{
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }

            Text {
                anchors.centerIn: parent
                text: "Protocol"
            }

            MouseArea{
                anchors.fill: parent
                onPressed: {
                    protocol.scale = 0.9;
                    pcap.md.sort_proto();
                }
                onReleased: {
                    protocol.scale = 1.0;
                }
            }

        }
        Rectangle {
            id: length

            width: root.lengthColumnWidth
            height: 30
            border.color: "#DDDDDD"
            border.width: 2

            Behavior on scale {
                NumberAnimation{
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }
            Text {
                anchors.centerIn: parent
                text: "Length"
            }

            MouseArea{
                anchors.fill: parent
                onPressed: {
                    length.scale = 0.9;
                    pcap.md.sort_len()
                }
                onReleased: {
                    length.scale = 1.0;
                }
            }
        }

        Rectangle {
            id: info

            width: root.infoColumnWidth
            height: 30
            border.color: "#DDDDDD"
            border.width: 2

            Behavior on scale {
                NumberAnimation{
                    duration: 80
                    easing.type: Easing.InOutQuad
                }
            }

            Text {
                anchors.centerIn: parent
                text: "Info"
            }

            MouseArea{
                anchors.fill: parent
                onPressed: {
                    info.scale = 0.9;
                }
                onReleased: {
                    info.scale = 1.0;
                }
            }
        }
    }



    ListView {
        id: l_view
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 30 // 스크롤바 공간 확보
        anchors.top: header.bottom
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        spacing: 5
        clip: true // 영역 밖 항목 잘라내기

        model: pcap.md

        // 스크롤바 추가
        ScrollBar.vertical: ScrollBar {
            active: true
            policy: ScrollBar.AlwaysOn

            contentItem: Rectangle {
                implicitWidth: 8
                radius: 4
                color: parent.pressed ? "#606060" : "#808080"

                // 마우스 올렸을 때 색상 변경
                opacity: parent.hovered ? 1.0 : 0.7
            }

            background: Rectangle {
                implicitWidth: 10
                color: "#E0E0E0"
                radius: 5
            }
        }

        delegate: Rectangle {
            width: l_view.width - 20
            height: 30
            color: get_color();
            border.color: ListView.isCurrentItem ? "#2196F3" : "#404040"
            border.width:ListView.isCurrentItem ? 2 : 1
            radius: 5

            Row {
                anchors.fill: parent
                spacing: header.spacing

                Text {
                    text: model.number
                    font.pixelSize: 14
                    width: root.noColumnWidth
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                } // No
                Text {
                    text: model.source
                    font.pixelSize: 14
                    width: root.sourceColumnWidth
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                } // Source
                Text {
                    text: model.destination
                    font.pixelSize: 14
                    width: root.destinationColumnWidth
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                } // Destination
                Text {
                    text: model.protocol
                    font.pixelSize: 14
                    width: root.protocolColumnWidth
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                } // Protocol
                Text {
                    text: model.length
                    font.pixelSize: 14
                    width: root.lengthColumnWidth
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                } // Length
                Text {
                    text: model.info
                    font.pixelSize: 14
                    width: root.infoColumnWidth
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight

                } // Info
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    l_view.currentIndex = index;
                }

                onDoubleClicked: {
                    if(capture_animation.isCapturing)
                    {
                        console.log("lock...");
                        return;
                    }

                    pcap.start_tree_md(model.number);
                    stk.push("PktView.qml",
                             {dump_idx: model.number,
                                 pcap: pcap});
                }
            }

            function get_color(){

                let proto = model.protocol

                if(ListView.isCurrentItem)
                {
                    return "#D0E8FF";
                }
                else if(proto === "TCP")
                {
                    return "#E7FBEA";
                }
                else if(proto === "UDP")
                {
                    return "#E6F3FF";
                }
                else if(proto === "ICMP")
                {
                    return "#FFE6F2";
                }
            }
        }
    }
}
