import QtQuick
import QtQuick.Controls
import Mini_Wireshark 1.0   // ← 정확히 이거

ApplicationWindow
{
    id: win
    width: 1920
    height: 1080
    visible: true
    title: qsTr("The Mini Wireshark");

        Pcap {
            id: pcap
        }


        StackView{
            id:stk
            anchors.fill: parent
            initialItem: "NicListView.qml";
            // initialItem: "TEST.qml";

            pushEnter: Transition {

            }
            popEnter: Transition {

            }
            pushExit: Transition {

            }
            popExit: Transition {

            }
            replaceEnter: Transition {

            }
            replaceExit: Transition {

            }

        }
}
