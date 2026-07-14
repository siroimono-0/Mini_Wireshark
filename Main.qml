import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Mini_Wireshark 1.0   // ← 정확히 이거

ApplicationWindow
{
    id: win
    width: Math.min(1180, Screen.desktopAvailableWidth * 0.92)
    height: Math.min(780, Screen.desktopAvailableHeight * 0.90)
    minimumWidth: 720
    minimumHeight: 520
    visible: true
    title: qsTr("The Mini Wireshark");
    color: "#3D3D3D"

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
