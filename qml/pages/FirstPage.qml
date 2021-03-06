
import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
  id: page1

  Column
  {
    anchors.bottom:idGrid.top
    width: parent.width
    Slider  {
      enabled: false
      id:idSliderTotalTime
      width: parent.width
      value: VedTimer.ElapsedTimeSliderValue
      valueText: "Elapsed time:" + VedTimer.TotalStr
    }

    Slider  {
      id:idSliderVolume
      objectName:"idSliderVolume"
      height:idSliderTotalTime.height
      onValueChanged: {
        VedTimer.setVolume(value)
      }

      width: parent.width
      valueText: "Volume:" + value.toFixed(2)
    }

    StepSlider  {
      id:idSliderInterval
      objectName:"idSliderInterval"
      width: parent.width
      onValueChanged: {
        VedTimer.setInterval(value)
      }
      //value:0.2
      valueText : "Intervall:" + VedTimer.IntervallStr
    }

    StepSlider  {
      id:idSliderCurrent
      objectName:"idSliderCurrentVal"
      Component.onCompleted: VedTimer.setCurrent(value)
      width: parent.width
      onValueChanged: {
        VedTimer.setCurrent(value)
      }
      value:1.0
      valueText:"Time to Fill:"+VedTimer.TimeToFillStr
    }
  }
  Grid
  {
    id:idGrid
    anchors.centerIn: parent
    spacing: 10

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

}




