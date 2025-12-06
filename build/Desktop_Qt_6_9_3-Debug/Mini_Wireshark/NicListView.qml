import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Mini_Wireshark 1.0   // ← 정확히 이거

Item {
    id: root
    width: 1500
    height: 900
    visible: false


    Item{
        id: root_item
        property string selectNic: ""
    }

    NicList{
        id: nic
    }

    Component.onCompleted:{
        root_item.selectNic = l_view.model.get_FirstNic();
    }

    Rectangle{
        id: rec_hello
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 50
        color: "#FAFAFA"  // 파스텔 하늘색
        border.color: "#404040"  // 하늘색 테두리
        border.width: 2
        radius: 10  // 둥근 모서리

        width: 200
        height: 40

        Text{
            text: "Mini Wireshark";
            // anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 20;
            // font.bold: true;
            // color: "gray"
        }
    }


    Rectangle{
        id: select_file
        anchors.left: rec_hello.right
        anchors.leftMargin: 10
        anchors.verticalCenter: rec_hello.verticalCenter

        color: "#FAFAFA"  // 파스텔
        border.color: "#404040"  // 하늘색 테두리
        border.width: 2
        radius: 20  // 둥근 모서리

        width: 120
        height: 40

        // 🔥 애니메이션용 속성
        property real normalScale: 1.0
        property real pressedScale: 0.94   // 눌리면 살짝 줄어듦

        // 🔥 상태 전환
        states: [
            State {
                name: "pressed"
                when: select_mouse.pressed
                PropertyChanges {
                    target: select_file
                    scale: select_file.pressedScale
                }
            }
        ]

        // 🔥 애니메이션 (부드럽게)
        transitions: [
            Transition {
                NumberAnimation {
                    properties: "scale, color"
                    duration: 120
                    easing.type: Easing.InOutQuad
                }
            }
        ]

        Text{
            text: "Open Pcap";
            // anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 20;
            // font.bold: true;
        }

        MouseArea{
            id: select_mouse
            anchors.fill: parent
            onClicked: {
                pacp_diaw.visible = true;
            }
        }

    }

    FileDialog{
        id: pacp_diaw
        title: "Pcap Filse Select";
        visible: false

        onAccepted: {
            // StackView에 접근해서 페이지 이동
            stk.push("CaptureView.qml",
                     {nicName: root_item.selectNic,
                      pcap_path: pacp_diaw.selectedFile});

        }
    }


    Text {
        id: input_text
        text: "Input Capther Filter ::"
        font.pixelSize: 18;
        color: "white"

        anchors.left: rec_hello.left
        anchors.leftMargin: 10
        anchors.top: rec_hello.bottom
        anchors.topMargin: 5
    }

    ComboBox{
        id: f_Box
        width: 150;

        anchors.left: input_text.right
        anchors.leftMargin: 5
        anchors.top: rec_hello.bottom
        anchors.topMargin: 8

        model: ["All",
            "WireLess Local",
            "Local",
            "Virtual"]

        // 항목 선택 시 실행되는 함수
        onActivated: function(index) {
            console.log("선택된 인덱스:", index)
            console.log("선택된 텍스트:", currentText)

            // 텍스트에 따라 다른 동작 실행
            if (currentText === "All") {
                // All 선택 시 실행할 코드
                nic.set_Nic(5);
                console.log("All 선택됨")
            }
            else if (currentText === "WireLess Local") {
                // WireLess Local 선택 시 실행할 코드
                nic.set_Nic(2);
                console.log("WireLess Local 선택됨")
            }
            else if (currentText === "Local") {
                // Local 선택 시 실행할 코드
                nic.set_Nic(3);
                console.log("Local 선택됨")
            }
            else if (currentText === "Virtual") {
                // Virtual 선택 시 실행할 코드
                nic.set_Nic(4);
                console.log("Virtual 선택됨")
            }
        }

        // ==========AI=============================================
        // ==========AI=============================================
        // 흰색 테마
        background: Rectangle {
            color: "white"
            border.color: "#CCCCCC"
            border.width: 1
            radius: 4
        }

        contentItem: Text {
            text: f_Box.displayText
            font: f_Box.font
            color: "black"
            verticalAlignment: Text.AlignVCenter
            leftPadding: 10
        }

        popup: Popup {
            y: f_Box.height
            width: f_Box.width
            padding: 1

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: f_Box.popup.visible ? f_Box.delegateModel : null
                currentIndex: f_Box.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            background: Rectangle {
                color: "white"
                border.color: "#CCCCCC"
                border.width: 1
                radius: 4
            }
        }

        delegate: ItemDelegate {
            width: f_Box.width
            contentItem: Text {
                text: modelData
                color: "black"
                font: f_Box.font
                verticalAlignment: Text.AlignVCenter
                leftPadding: 10
            }
            background: Rectangle {
                color: {
                    if (highlighted) return "#BBDEFB"  // 선택된 항목
                    else if (hovered) return "#E3F2FD"  // 마우스 올렸을 때
                    else return "white"  // 기본
                }
            }

            // hover 효과 활성화
            hoverEnabled: true
        }
        // ==========AI=============================================
        // ==========AI=============================================


    }


    ListView{
        id: l_view
        width: 800
        height: 500

        anchors.left: parent.left
        anchors.leftMargin: 70
        anchors.top: parent.top
        anchors.topMargin: 130

        spacing: 5  // 항목 사이 간격

        model:nic

        // 하이라이트 활성화
        focus: true
        highlightFollowsCurrentItem: true

        delegate: Rectangle{
            id:nic_rec
            width: 500
            height: 30

            // 선택 여부에 따른 배경색
            color: {
                if (ListView.isCurrentItem) {
                    return "#D0E8FF"  // 선택된 항목 (하이라이트)
                }
                else {
                    if (model.type === 2) return "#FAFAFA"
                    else if (model.type === 3) return "#FAFAFA"
                    else if (model.type === 4) return "#FAFAFA"
                    else return "#FAFAFA"
                }
            }

            // 선택 여부에 따른 테두리 색상
            border.color: {
                if (ListView.isCurrentItem) {
                    return "#2196F3"  // 선택된 항목 테두리 (파란색)
                }
                else {
                    if (model.type === 2) return "#404040"
                    else if (model.type === 3) return "#404040"
                    else if (model.type === 4) return "#404040"
                    else return "#404040"
                }
            }
            border.width: ListView.isCurrentItem ? 3 : 2  // 선택된 항목은 테두리 더 두껍게
            radius: 10

            Text {
                text: model.text
                font.pixelSize: 18;
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                color: ListView.isCurrentItem ? "#1976D2" : "black"  // 선택된 항목 텍스트 색상
            }

            MouseArea{
                id: nic_mouse
                anchors.fill: parent

                onClicked: {
                    l_view.currentIndex = index  // 클릭한 항목을 현재 항목으로 설정
                    root_item.selectNic = model.text;
                }

                onDoubleClicked: {
                    // StackView에 접근해서 페이지 이동
                    stk.push("CaptureView.qml",
                             {nicName: model.text})
                }
            }
        }
    }
}


/*Component.onCompleted: {
    console.log("parent:", parent)
    console.log("parent width:", parent.width)
    console.log("parent height:", parent.height)
}
*/
