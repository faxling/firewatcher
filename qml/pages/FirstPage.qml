
import QtQuick 2.0
import Sailfish.Silica 1.0



Page {
    objectName: "idFirstId"


    id: page1

    Grid
    {
        id:idGrid
        anchors.centerIn: parent
   //     spacing: 10

        columns: 2

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


            text:
            {
                switch (nFireState)
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

    Image {
        anchors.horizontalCenter:  parent.horizontalCenter
        anchors.top : idGrid.bottom
        id: image1
        source: "spis_large.png"
    }
    Slider  {
        anchors.bottom: idGrid.top
        id:idSliderCurrent

        onValueChanged: {
            VedTimer.setCurrent(value)
        }

        objectName:"idCurrentVal"
        width: parent.width
        value:1.0
        valueText:"Time to Fill:"+VedTimer.TimeToFill
    }

    Slider  {
        anchors.bottom: idSliderCurrent.top
        id:idSlider
        onValueChanged: {
            valueText = "Intervall:" + VedTimer.setInterval(value)
        }

        width: parent.width
        value: 0.2
        valueText : "Intervall:" + VedTimer.setInterval(value)
    }

    Slider  {
        anchors.bottom: idSlider.top
        id:idSliderVolume

        onValueChanged: {
            VedTimer.setVolume(value)
        }

        width: parent.width
        value: 0.2
        valueText: "Volume:" + value.toFixed(2)
    }

    Component.onCompleted: {

        // idIntervalText.text = VedTimer.setInterval(idSlider.value)
        //  idValText.text = idIntervalText.text
    }
}




