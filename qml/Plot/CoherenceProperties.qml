/**
 *  OSM
 *  Copyright (C) 2019  Pavel Smokotnin

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

import "../" as Root
import SourceModel 1.0

Item {
    id: chartProperties
    property var dataObject

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

    RowLayout {
        spacing: 0

        SpinBox {
            value: dataObject.xmin
            onValueChanged: dataObject.xmin = value
            from: dataObject.xLowLimit
            to: dataObject.xHighLimit
            editable: true
            implicitWidth: 170
            Layout.fillWidth: true

            ToolTip.visible: hovered
            ToolTip.text: qsTr("x from")

            textFromValue: function(value, locale) {
                return Number(value) + "Hz"
            }

            valueFromText: function(text, locale) {
                return parseInt(text)
            }
        }

        SpinBox {
            value: dataObject.xmax
            onValueChanged: dataObject.xmax = value
            from: dataObject.xLowLimit
            to: dataObject.xHighLimit
            editable: true
            implicitWidth: 170
            Layout.fillWidth: true

            ToolTip.visible: hovered
            ToolTip.text: qsTr("x to")

            textFromValue: function(value, locale) {
                return Number(value) + "Hz"
            }

            valueFromText: function(text, locale) {
                return parseInt(text)
            }
        }

        Root.FloatSpinBox {
            min: dataObject.yLowLimit
            max: dataObject.yHighLimit
            value: dataObject.ymin
            tooltiptext: qsTr("y from")
            onValueChanged: dataObject.ymin = value
            implicitWidth: 170
            Layout.fillWidth: true
        }

        Root.FloatSpinBox {
            min: dataObject.yLowLimit
            max: dataObject.yHighLimit
            value: dataObject.ymax
            tooltiptext: qsTr("y to")
            onValueChanged: dataObject.ymax = value
            implicitWidth: 170
            Layout.fillWidth: true
        }

        Button {
            text: qsTr("Save Image");
            implicitWidth: 120
            onClicked: fileDialog.open();
            ToolTip.visible: hovered
            ToolTip.text: qsTr("save chart to a file")
        }
    }
    RowLayout {
        spacing: 0

        Root.TitledCombo {
            title: qsTr("ppo")
            tooltip: qsTr("points per octave")
            implicitWidth: 170
            model: [3, 6, 12, 24, 48]
            currentIndex: {
                var ppo = dataObject.pointsPerOctave;
                model.indexOf(ppo);
            }
            onCurrentIndexChanged: {
                var ppo = model[currentIndex];
                dataObject.pointsPerOctave = ppo;
            }
        }

        Rectangle {
            width: 5
        }

        ComboBox {
            id: type
            implicitWidth: 120
            model: ["normal", "squared"]
            currentIndex: dataObject.type
            ToolTip.visible: hovered
            ToolTip.text: qsTr("value type")
            onCurrentIndexChanged: dataObject.type = currentIndex;
        }

        RowLayout {
            Layout.fillWidth: true
        }

        Root.TitledCombo {
            title: qsTr("filter")
            tooltip: qsTr("show only selected source")
            model: SourceModel {
                addNone: true
                list: sourceList
            }
            Layout.preferredWidth: 280
            currentIndex: { model.indexOf(dataObject.filter) }
            textRole: "title"
            valueRole: "source"
            onCurrentIndexChanged: {
                dataObject.filter = model.get(currentIndex);
            }
        }

        FileDialog {
            id: fileDialog
            selectExisting: false
            title: "Please choose a file's name"
            folder: shortcuts.home
            defaultSuffix: "png"
            onAccepted: {
                dataObject.parent.grabToImage(function(result) {
                    result.saveToFile(dataObject.parent.urlForGrab(fileDialog.fileUrl));
                });
            }
        }
    }
  }
}
