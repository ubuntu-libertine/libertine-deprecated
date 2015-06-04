/**
 * @file libertine.qml
 * @brief Libertine app main view.
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
import QtQuick 2.4
import U1db 1.0 as U1db


Item {

    U1db.Index {
        id: byContainerId
        database: configDB
        expression: [ "container" ]
    }

    U1db.Query {
        id: containerQuery
        index: byContainerId
        query: ["*"]
    }

    function getContainers() {
        return containerQuery.results
    }

    function areContainersAvailable() {
        var containers = getContainers()
        return containers.length > 0
    }

    function addNewContainer(imageSource) {
        print("addNewContainer(id='" + imageSource.id + "', '" + imageSource.name + "')")
        var newConfig = {
            "container": {
                "id": imageSource.id,
                "name": imageSource.name,
                "status": "NEW"
            }
        }
        configDB.putDoc(newConfig)
    }
}
