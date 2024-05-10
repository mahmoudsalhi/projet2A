import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6

Rectangle {
    width: 800
    height: 600

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(34.7151, 10.8027) // Center the map at the specified coordinates (Tunisia)
        zoomLevel: 6 // Zoom level to get a better view

        // Mouse area to detect clicks on the map
        MouseArea {
            anchors.fill: parent
            onClicked: {
                // Get the coordinates of the click
                var coordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y));
                var latitude = coordinate.latitude;
                var longitude = coordinate.longitude;
                console.log("Latitude:", latitude);
                console.log("Longitude:", longitude);
                var coordinateString = latitude.toString() + ", " + longitude.toString();
                console.log("Combined coordinate string:", coordinateString);
                ClipboardHelper.setText(coordinateString);

                // Create a MapQuickItem at the clicked coordinates
                mapQuickItem.coordinate = QtPositioning.coordinate(latitude, longitude); // Coordinates of a specific location in Tunisia
            }
        }

        // Add a red mark at specific coordinates in Tunisia
        MapQuickItem {
            id: mapQuickItem
            anchorPoint.x: sourceItem.width / 2
            anchorPoint.y: sourceItem.height
            sourceItem: Image {
                source: "qrc:/markerImage"
                width: 20
                height:20
            }
        }
    }
}
