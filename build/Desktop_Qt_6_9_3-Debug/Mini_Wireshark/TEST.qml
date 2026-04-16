import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Mini_Wireshark 1.0

Item {
    id: root

    width: 1000
    height: 700

    // Rec_icon{
        // anchors.centerIn: parent
        // width: 500
        // height: 500
        // img_source: "img/start_.svg";
    // }
    Image {
        id: awda
        anchors.centerIn: parent
        width: 500
        height: 500
        source: "img/start_.svg";
    }
}
