import QtQuick 2.0
import fluxie.gp.performance 1.0

Rectangle {

    id: control
    property string title: "Kop"
    property var source
    property var content

    onContentChanged: if( loader.item ) loader.item.content = control.content

    Text {
        id: label
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        color: "black"
        text: title
        width: parent.width
    }

    Loader {

        id: loader
        anchors.top: label.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        source: control.source        

//        onLoaded: loader.item.content = control.content
    }

}


