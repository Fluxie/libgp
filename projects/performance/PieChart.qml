import QtQuick 2.0
import QtCharts 2.2

ChartView {


    property var content: ({})
    property var slices: null

    anchors.fill: parent
    theme: ChartView.ChartThemeDark
    antialiasing: true


//![1]
//![2]
    PieSeries {
        id: pieSeries

    }

    onContentChanged: {

        // Create the slices, assume the order of the slices do not change dyanmically.
        if( ! content )
        {
            // Clear previous slices.
            pieSeries.clear();
            slices = [];
        }
        else if( slices === null || slices.length !== content.length ) {

            // Clear previous slices.
            pieSeries.clear();
            slices = [];

            // Create new slices.
            for( var i = 0; i < content.length; ++i ) {
                slices[ content[ i ].label ] = pieSeries.append( content[ i ].label, content[ i ].value )
            }
        }
        else {

            // Only update the values of the slices.
            for( var i = 0; i < content.length; ++i ) {
                slices[ content[ i ].label ].value = content[ i ].value;
            }

        }

    }


}
