import QtQuick 2.4
import QtQuick.Layouts 1.0
import Ubuntu.Components 1.2


UbuntuListView {
    id: listView
    //anchors.fill: parent
    anchors.top: parent.searchString.bottom
    //anchors {
    //    top: parent.bottom
    //    margins: units.gu(3)
    //}
    
    delegate: ListItem {
        Label {
            text: model.package
        }
    }
}
