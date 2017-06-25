import QtQuick 2.7
import fluxie.gp.performance 1.0

Rectangle {

    id: charts

    property variant dataFromMonitor: dataMonitor.data
    property DataPacket test

    GridView {
        id: gridView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        cellWidth: width/2
        cellHeight: height/2

        model: dataModel
        delegate: ChartsControl {
            width: gridView.cellWidth
            height: gridView.cellHeight
            title: model.title
            source: model.control
            content: model.content
        }
    }

}


