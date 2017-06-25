import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id: statistics
    property variant content: ({})

    TextValue {
        id: allocations
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: 20
        name: "Allocations"
    }


    ListModel {
        id: customDataModel
    }


    ListView {
        id: customDataView


        anchors {
            left: parent.left
            right: parent.right
            top: allocations.bottom
            bottom: parent.bottom
        }

        model: customDataModel
        delegate: TextValue {

            anchors {
                left: parent.left
                right: parent.right
            }
            height: 20

            name: model.name
            value: model.value
        }

    }

    // Updates the list model based on the custom data.
    onContentChanged: {

        customDataModel.clear();
        if( ! content )
            return;

        // Update the number of allocations.
        if( content.allocations )
            allocations.value = content.allocations;

        var customData = content.customData;
        for( var dataItem in customData )
        {
            // Convert to model item.
            var modelItem = {
                name: dataItem,
                value: customData[ dataItem ]
            };
            customDataModel.append( modelItem )
        }
    }
}
