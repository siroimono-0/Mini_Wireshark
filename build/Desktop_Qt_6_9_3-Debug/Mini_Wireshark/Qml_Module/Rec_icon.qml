import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Item {
    id: root
    property var img_source;

    Rectangle{

        color: "transparent"
        border.color: "#FFFFFF"
        border.width: 2
        anchors.fill: parent

        layer.enabled: true

        layer.effect: MultiEffect{
            shadowEnabled: true;
            shadowBlur: 1
            shadowColor: "#FFFFFF"
            shadowOpacity: 1
            shadowVerticalOffset: 0
            shadowHorizontalOffset: 0
        }
    }

    Image {
        id: img
        source: root.img_source
        // color: root.img_color
        anchors.fill: parent
        anchors.margins: 2
        fillMode: Image.PreserveAspectFit
    }
}
