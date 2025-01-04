import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

import QRScanner

Window {
    width: 640
    height: 480
    visible: true

    MediaDevices {
        id: mediaDevices
    }

    CaptureSession {
        camera: Camera {
            id: camera
            cameraDevice: mediaDevices.defaultVideoInput
            active: true

            focusMode: Camera.FocusModeAutoNear
            customFocusPoint: Qt.point(0.2, 0.2) // Focus relative to top-left corner
        }
        videoOutput: videoOutput
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent

        Component.onCompleted: {
            Driver.setVideoSink(videoOutput.videoSink)
        }

    }

    Popup {
        id: wifiFound

        property var ssid
        property var password

        anchors.centerIn: parent
        width: parent.width / 2
        height: parent.height / 3
        visible: false
        modal: true

        background: Rectangle {
            color: "black"
            opacity: 0.8
            border.width: 1
            border.color: "white"
            radius: 8
        }
        ColumnLayout
        {
            anchors.fill: parent

            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true

                text: "Wifi network found with \nSSID: " + wifiFound.ssid + "\npassword: " + wifiFound.password
                color: "white"
                font.pixelSize: 24
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                Button {
                    text: "Close"
                    onClicked: {
                        wifiFound.visible = false
                    }
                }
                Button {
                    text: "Connect"
                    onClicked: {
                        Driver.connectToWifi(wifiFound.ssid, wifiFound.password)
                        wifiFound.visible = false
                    }
                }
            }
        }

    }

    Popup {
        id: connectionState
        property var state
        width: parent.width / 2
        height: parent.height / 3
        anchors.centerIn: parent
        visible: false
        modal: true
        background: Rectangle {
            color: switch (connectionState.state)
                {
                    case Driver.Connecting:
                        return "black"
                    case Driver.Connected:
                        return "green"
                    case Driver.Disconnected:
                        return "red"
                }
            opacity: 0.8
            border.width: 1
            border.color: "white"
            radius: 8
        }
        ColumnLayout
        {
            anchors.fill: parent
            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: switch (connectionState.state)
                {
                    case Driver.Connecting:
                        return "Connecting to wifi network..."
                    case Driver.Connected:
                        return "Connected to wifi network \n" +  wifiFound.ssid
                    case Driver.Disconnected:
                        return "Failed To Connect to wifi network"
                }
                color: "white"
                font.pixelSize: 24
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

    }

    Connections {
        target: Driver

        function onWifiNetworkFound(ssid, password) {
            wifiFound.ssid = ssid
            wifiFound.password = password
            wifiFound.visible = true
        }

        function onConnectingStateChanged(state) {
            connectionState.state = state
            connectionState.visible = true
        }
    }
}
