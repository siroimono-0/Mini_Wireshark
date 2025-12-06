import QtQuick
import QtQuick.Controls
import Mini_Wireshark 1.0   // ← 정확히 이거

ApplicationWindow
{
    id: win
    width: 1500
    height: 900
    visible: true
    title: qsTr("The Mini Wireshark");

        Pcap {
        id: pcap
    }


    StackView{
        id:stk
        anchors.fill: parent
        initialItem: "NicListView.qml";
    }
}
