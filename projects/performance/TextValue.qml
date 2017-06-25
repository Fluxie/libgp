import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {

    property string name
    property string value

    Text {
        id: nameField

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
        width: 100

        text: name + ":"
    }

    Text {
        id: valueField
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: nameField.right
        }
        horizontalAlignment: Text.AlignRight
        text: value
    }

}
