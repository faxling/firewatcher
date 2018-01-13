
import QtQuick 2.0
import Sailfish.Silica 1.0



Page {
    objectName: "idFirstId"


    id: page1

    Image {
        anchors.centerIn: parent
        id: image1
        source: "spis_large.png"
    }

    Grid
    {
        id:idGrid
        anchors.centerIn: parent
        spacing: 10

        columns: 2

        Label {
            width:idRestbtn.width
            horizontalAlignment : Text.AlignHCenter
            id: idValLabel2
            text: "Time Left"
        }
        Label {
            width:idRestbtn.width
            horizontalAlignment : Text.AlignHCenter

            text: "0:0:3"
            id:idValText
        }
        Button {

            id:idRestbtn
            text: "Reset"
            onClicked: VedTimer.resetTimer()
        }

        Button {
            property int nFireState : 0
            color:
                switch (nFireState)
                {
                case 0:
                    "chartreuse"
                    break;
                case 1:
                    "DimGrey"
                    break;
                case 2:
                    "OrangeRed"
                    break;
                case 3:
                    "Red"
                }
            objectName: "idStartBtn"

            Text {
                id:idStartBtn
                anchors.centerIn: parent
                text:
                    switch (parent.nFireState)
                    {
                    case 0:
                        "Start"
                        break;
                    case 1:
                        "Continue"
                        break;
                    case 2:
                        "Pause"
                        break;
                    case 3:
                        "Fill her up!"
                    }

            }

            onClicked: {
                VedTimer.startTimer()
            }

        }
    }
    Slider  {
       /// anchors.bottom: idGrid.top
        id:idSlider
        onValueChanged: {
            valueText = "Intervall:" + VedTimer.setInterval(value)
        }

        width: parent.width
        value: 0.2
        valueText : "Intervall:" + VedTimer.setInterval(value)
    }

    Slider  {
       //  anchors.bottom: idSlider.top
        id:idSliderVolume

        onValueChanged: {
            VedTimer.setVolume(value)
        }

        width: parent.width
        value: 0.2
        valueText: "Volume:" + value.toFixed(2)
    }

    Slider  {
      //  anchors.bottom: idSlider.top
        id:idSliderCurrent

        onValueChanged: {
            VedTimer.setCurrent(value)
        }

        objectName:"idCurrentValText"
        width: parent.width
    }

    Component.onCompleted: {

        // idIntervalText.text = VedTimer.setInterval(idSlider.value)
        //  idValText.text = idIntervalText.text
    }
}




