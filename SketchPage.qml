import QtQuick
import QtQuick.Shapes
import QtQuick.Controls as QC
import QtQuick.Layouts
import QtQuick.Window
import CaveWhereSketch
import cavewherelib

StandardPage {
    id: sketchPageId
    clip: true

    // property double dpi: windowId.dpi

    // onDpiChanged: {
    //     console.log("DPI changed:" + dpi)
    // }

    PenLineModel {
        id: penModel

        viewScale: containerId.scale

        onCurrentStrokeWidthChanged: {
            console.log("onCurrentStrokeWidthChanged!" + penModel.currentStrokeWidth + (penModel.currentStrokeWidth.toFixed(1) == 2.5))
            featuresId.checked = windowId.fuzzyEquals(penModel.currentStrokeWidth, 2.5);
        }
    }

    MovingAverageProxyModel {
        id: movingAverageProxyModelId
        sourceModel: penModel
    }

    PainterPathModel {
        id: painterPathModel
        // penLineModel: penModel
        penLineModel: movingAverageProxyModelId
    }

    WorldToScreenMatrix {
        id: worldToScreenId
        pixelDensity: Screen.pixelDensity //units = mm
        scale.scaleNumerator.value: 1
        scale.scaleDenominator.value: 250
    }

    QC.ButtonGroup {
        id: strokeWidthGroupId
    }

    QC.GroupBox {
        title: "Stroke Width"
        z: 1
        anchors.right: parent.right


        ColumnLayout {
            id: columnLayoutId
            QC.RadioButton {
                id: wallId
                text: "Wall"
                QC.ButtonGroup.group: strokeWidthGroupId
                onClicked: {
                    penModel.currentStrokeWidth = 4.0
                }

                Binding {
                    wallId.checked: windowId.fuzzyEquals(penModel.currentStrokeWidth, 4.0)
                }
            }

            QC.RadioButton {
                id: featuresId
                text: "Features"
                QC.ButtonGroup.group: strokeWidthGroupId
                onClicked: {
                    penModel.currentStrokeWidth = 2.5
                }

                checked: windowId.fuzzyEquals(penModel.currentStrokeWidth, 2.5)
            }

            QC.RadioButton {
                text: "With Pressure"
                QC.ButtonGroup.group: strokeWidthGroupId
                onClicked: {
                    penModel.currentStrokeWidth = -1.0;
                }
            }

            QC.Button {
                id: biggerId
                text: "Bigger add 0.1 to " + penModel.currentStrokeWidth.toFixed(2)
                onClicked: {
                    penModel.currentStrokeWidth += 0.1
                }
            }

            GroupBox {
                ColumnLayout {
                    QC.Button {
                        id: undoId
                        enabled: penModel.undoStack.canUndo
                        text: "Undo"
                        onClicked: {
                            penModel.undoStack.undo();
                        }
                    }

                    QC.Button {
                        id: redoId
                        enabled: penModel.undoStack.canRedo
                        text: "Redo"
                        onClicked: {
                            penModel.undoStack.redo();
                        }
                    }

                    QC.Button {
                        id: clearId
                        text: "Clear"
                        enabled: penModel.canClearUndoStack
                        onClicked: {
                            penModel.clearUndoStack();
                        }
                    }

                }
            }
        }
    }

    // Rectangle {
    //     color: "red"
    //     width: 100
    //     height: 100
    //     x: 100
    //     y: 100

    //     TapHandler {
    //         target: parent
    //         grabPermissions: PointerHandler.CanTakeOverFromAnything
    //         // acceptedDevices: handler.acceptedDevices |
    //         gesturePolicy: TapHandler.ReleaseWithinBounds
    //         onTapped: {
    //             console.log("Tapped!")
    //         }
    //     }
    // }

    // QC.Button {

    //     id: biggerId2
    //     z: 1
    //     x: 0
    //     y: 400
    //     text: "Bigger add 0.1 to " + penModel.currentStrokeWidth.toFixed(2)
    //     onClicked: {
    //         penModel.currentStrokeWidth += 0.1
    //     }
    // }

    // Rectangle {
    //     id: tapRectangleId
    //     z: 1
    //     color: "red"
    //     width: 100
    //     height: 100
    //     x: 250
    //     y: 300
    //     opacity: 0.5
    //     property int count: 0

    //     Text {
    //         text: "Tap count:" + tapRectangleId.count
    //     }

    //     TapHandler {
    //         // target: parent
    //         // grabPermissions:
    //         // grabPermissions: PointerHandler.CanTakeOverFromAnything
    //         // acceptedDevices: handler.acceptedDevices |
    //         gesturePolicy: TapHandler.WithinBounds
    //         onTapped: {
    //             console.log("Tapped!")
    //             tapRectangleId.count++;4
    //         }
    //     }
    // }
    // Rectangle {
    //     id: tapRectangleId2
    //     z: 1
    //     color: "green"
    //     width: 100
    //     height: 100
    //     x: 300
    //     y: 350
    //     opacity: 0.5
    //     property int count: 0

    //     Text {
    //         text: "Tap count:" + tapRectangleId2.count
    //     }

    //     TapHandler {
    //         // target: parent
    //         // grabPermissions: PointHandler.TakeOverForbidden
    //         // grabPermissions: PointerHandler.CanTakeOverFromAnything
    //         // acceptedDevices: handler.acceptedDevices |
    //         gesturePolicy: TapHandler.WithinBounds
    //         acceptedDevices:  PointerDevice.Stylus | PointerDevice.Mouse
    //         onTapped: {
    //             console.log("Tapped!")
    //             tapRectangleId2.count++;4
    //         }
    //     }
    // }


    PinchHandler {
        parent: sketchPageId
        target: containerId
        rotationAxis.enabled: false
        // scaleAxis.enabled: false
    }

    ExclusivePointHandler {
    // PointHandler {
        id: handler

        property double pressureScale: 10.0

        property point _mappedHandlerPoint

        //This works for ipad over sidecar on macOS
        //See this bug: https://bugreports.qt.io/browse/QTBUG-80072
        //acceptedDevices: PointerDevice.Unknown

        //Works for ipad, windows 11 with pen, android with spen
        // acceptedButtons: Qt.LeftButton
        acceptedDevices: PointerDevice.Stylus | PointerDevice.Mouse | PointerDevice.TouchPad //| PointerDevice.TouchScreen
        // acceptedPointerTypes: PointerDevice.Pen

        // grabPermissions: PointerHandler.ApprovesTakeOverByAnything //PointerHandler.CanTakeOverFromHandlersOfSameType | PointerHandler.ApprovesTakeOverByAnything

        cursorShape: Qt.BlankCursor
        target: containerId
        // target: Rectangle {
        //     parent: containerId
        //     color: "red"
        //     visible: handler.active
        //     x: handler._mappedHandlerPoint.x - width / 2
        //     y: handler._mappedHandlerPoint.y - height / 2
        //     width: handler.pressureScale * handler.point.pressure;
        //     height: width;
        //     radius: width / 2
        // }
        // parent: sketchPageId
        // parent: containerId
        // target: containerId
        parent: sketchPageId

        onActiveChanged: {
            // console.log("Active changed!" + active)
            if(active) {
                painterPathModel.activeLineIndex = penModel.rowCount();
                penModel.addNewLine();
            } else {
                penModel.finishNewLine();
            }
        }

        onPointChanged: {
            if(active && handler.point.pressure > 0.0) {
                //console.log("Point.pressure:" + handler.point.position + " " + handler.point.pressure + active)
                _mappedHandlerPoint = sketchPageId.mapToItem(containerId, handler.point.position);

                let penPoint = penModel.penPoint(_mappedHandlerPoint, handler.point.pressure)
                penModel.addPoint(painterPathModel.activeLineIndex, penPoint)
            }
        }

        // onGrabChanged: (transition, point) => {
        //     console.log("Grab changed:" + transition + " Exculive:" + PointerDevice.GrabExclusive + " Ungrab:" + PointerDevice.UngrabExclusive + " passive:" + PointerDevice.GrabPassive + " passive ungrab:" + PointerDevice.UngrabPassive)
        // }
    }

    WheelHandler {
        target: containerId
        property: "scale"
        targetScaleMultiplier: 1.1
    }


    DragHandler {
        // target: containerId
        target: null
        acceptedButtons: Qt.RightButton
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad

        dragThreshold: 0

        property point lastPosition;

        function containerPos(scenePosition) {
            return containerId.parent.mapFromGlobal(centroid.scenePosition.x, centroid.scenePosition.y)
        }

        onActiveChanged: {
            if(active) {
                lastPosition = containerPos(centroid.scenePosition);
            }
        }

        onCentroidChanged: {
            if(active) {
                let newPosition = containerPos(centroid.scenePosition);
                let deltaX = newPosition.x - lastPosition.x
                let deltaY = newPosition.y - lastPosition.y

                containerId.x += deltaX
                containerId.y += deltaY

                lastPosition = newPosition
            }
        }
    }

    Item {
        id: containerId
        // width: 1000
        // height: 1000

        // property Qt.point pos:
        property rect viewport; //: sketchPageId.mapToItem(containerId, 0, 0, sketchPageId.width, sketchPageId.height)

        function updateViewport() {
            viewport = sketchPageId.mapToItem(containerId, 0, 0, sketchPageId.width, sketchPageId.height)
        }

        // onViewportChanged: {
        //     console.log("Viewport changed:" + viewport)
        // }

        //Start off in the center, PinchHandler remove this binding
        x: sketchPageId.width * 0.5
        y: sketchPageId.height * 0.5

        onScaleChanged: {
            // console.log("Scale changed:" + scale)
            updateViewport();
        }
        onXChanged: updateViewport();
        onYChanged: updateViewport();

        InfiniteGrid {
            gridOrigin: Qt.point(containerId.x, containerId.y);
            viewScale: containerId.scale
            worldToScreenMatrix: worldToScreenId.matrix
            viewport: containerId.viewport
        }

        ShapePathInstantiator {
            anchors.fill: parent
            model: RootDataSketch.centerlinePainterModel
            // shape: centerline
        }

        ShapePathInstantiator {
            anchors.fill: parent
            model: painterPathModel
            // shape: shapeId
        }
    }

}
