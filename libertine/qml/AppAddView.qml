/**
 * @file AppAddView.qml
 * @brief Libertine app add view
 */
/*
 * Copyright 2015 Canonical Ltd
 *
 * Libertine is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3, as published by the
 * Free Software Foundation.
 *
 * Libertine is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import Libertine 1.0
import QtQuick 2.4
import QtQuick.Layouts 1.0
import Ubuntu.Components 1.2


Page {
    id: appAddView
    title: i18n.tr("Install Apps")

    ColumnLayout {
        id: column
        spacing: units.gu(2)
        anchors {
            fill: parent
        }

        Label {
            id: searchPackageMessage

            visible: false

            anchors {
                top: parent.top
                left: parent.left
                margins: units.gu(1)
            }

            Layout.fillWidth: true
            wrapMode: Text.Wrap

            text: i18n.tr("Please enter a package name to search for:")
        }

        TextField {
            id: searchInput

            visible: false

            anchors {
                top: searchPackageMessage.bottom
                left: parent.left
                margins: units.gu(1)
            }
            height: units.gu(4.5)
            width: parent.width - anchors.margins * 2

            onAccepted: {
                packageListModel.clear()
                listView.forceLayout()
                searchPackage(searchInput.text)
            }
        }

        Label {
            id: enterPackageMessage

            visible: false

            anchors {
                top: parent.top
                left: parent.left
                margins: units.gu(1)
            }

            Layout.fillWidth: true
            wrapMode: Text.Wrap

            text: i18n.tr("Please enter the exact package name of the app to install:")
        }

        TextField {
            id: appName

            visible: false

            anchors {
                top: enterPackageMessage.bottom
                left: parent.left
                margins: units.gu(1)
            }
            height: units.gu(4.5)
            width: parent.width - anchors.margins * 2

            onAccepted: {
                if (!containerConfigList.isAppInstalled(mainView.currentContainer, text)) {
                    containerAppsList.addNewApp(mainView.currentContainer, text)
                    installPackage()
                    appInstallMessage.text = i18n.tr("Installing ") + text + i18n.tr("...")
                    appInstallMessage.visible = true
                    appName.text = ""
                }
                else {
                    appInstallMessage.text = i18n.tr("Package ") + text + i18n.tr(" already installed. Please try a different package name.")
                    appInstallMessage.visible = true
                    appName.text = ""
                }
            }
        }

        Label {
            id: appInstallMessage

            visible: false

            anchors {
                top: appName.bottom
                margins: units.gu(3)
            }
            height: units.gu(4.5)
        }

        ListModel {
            id: packageListModel
        }

        UbuntuListView {
            id: listView
            anchors {
                top: searchInput.bottom
                bottom: parent.bottom
                right: parent.right
                left: parent.left
                topMargin: units.gu(1)
            }
            clip: true
            model: packageListModel

            delegate: ListItem {
                Label {
                    anchors {
                        left: parent.left
                        leftMargin: units.gu(1)
                    }
                    text: model.package
                }
            }
        }
    }

    head.actions: [
        Action {
            iconName: "search"
            onTriggered: {
                if (enterPackageMessage.visible) {
                    enterPackageMessage.visible = false
                    appName.visible = false
                    appName.text = ""
                }
                searchPackageMessage.visible = true
                searchInput.visible = true
                searchInput.forceActiveFocus()
            }
        },
        Action {
            iconName: "settings"
            onTriggered: {
                if (searchPackageMessage.visible) {
                    packageListModel.clear()
                    listView.forceLayout()
                    searchPackageMessage.visible = false
                    searchInput.visible = false
                    searchInput.text = ""
                }
                enterPackageMessage.visible = true
                appName.visible = true
                appName.forceActiveFocus()
            }
        }
    ]

    function installPackage() {
        var comp = Qt.createComponent("ContainerManager.qml")
        var worker = comp.createObject()
        worker.containerAction = ContainerManagerWorker.Install
        worker.containerId = mainView.currentContainer
        worker.data = appName.text
        worker.finishedInstall.connect(finishedInstall)
        worker.start()
    }

    function finishedInstall(result, error_msg) {
        if (result) {
            if (appAddView) {
                pageStack.pop()
            }
        }
        else {
            if (appAddView) {
                appInstallMessage.text = error_msg
                appInstallMessage.visible = true
            }
        }
    }

    function searchPackage(search_string) {
        var comp = Qt.createComponent("ContainerManager.qml")
        var worker = comp.createObject()
        worker.containerAction = ContainerManagerWorker.Search
        worker.containerId = mainView.currentContainer
        worker.data = search_string
        worker.finishedSearch.connect(finishedSearch)
        worker.start()
    }

    function finishedSearch(result, packageList) {
        for (var i = 0; i < packageList.length; ++i)
        {
            packageListModel.append({"package": packageList[i]})
        }
    }       
}
