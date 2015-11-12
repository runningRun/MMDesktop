import QtQuick 2.4
import QtMultimedia 5.0
import QtQuick.Window 2.2


Rectangle{
    id: rootDelegateItem;

    property var senderName: ""
    property var msgContent: "";
    property string pictureFilePath: "";
    property var audioFilePath: "";
    property var videoFilePath: "";
    property int msgType: -1;
    property int direction: -1;


    width: parent.width;
    color: "#e0e0e0";
    height: (getHeight(msgType) > 52) ?　getHeight(msgType) : 56;

    function getHeight(tmpMsgtype)
    {
        var iHeight = nicknametxt.height+getRelativeComponentHeight(tmpMsgtype);
        return iHeight;
    }

    function getRelativeComponentHeight(tmpMsgtype)
    {
        var tmpHeight = 0;
        switch (tmpMsgtype)
        {
        case 0:
            tmpHeight = msgContentRect.height + 10;
            break;
        case 1:
            tmpHeight = pictureRect.height + 10;
            break;
        case 2:
            tmpHeight = audioRect.height + 10;
            break;
        case 3:
            tmpHeight = videoRect.height + 10;
            break;
        }
        return tmpHeight;
    }

    Rectangle
    {
        id: headPortraitRect;
        width: 48;
        height: 48;
        anchors.left: parent.left;
        anchors.leftMargin: (0 == direction) ? parent.width - 50 : 4;
        anchors.top: parent.top;
        anchors.topMargin: 4;

        Image {
            id: headPortraitImg;
            source: "res/head_%1.png".arg(direction == 0 ? 0 : 1);
            anchors.centerIn: headPortraitRect;
            anchors.fill: headPortraitRect;
        }
    }
    Text {
        id: nicknametxt
        text: senderName
        width: 100
        clip: true
        height: 20
        anchors.left: (direction==0)?headPortraitRect.left:headPortraitRect.right
        anchors.leftMargin: (direction==0)?-100:4
        anchors.top: parent.top
        anchors.topMargin: 4
        horizontalAlignment: (direction==0)?Text.AlignRight:Text.AlignLeft

    }

    //文本信息框
    Rectangle
    {
        id: msgContentRect;
        width: (msgContentTxt.paintedWidth + 7 > rootDelegateItem.width - 70) ? rootDelegateItem.width - 70: msgContentTxt.paintedWidth + 7;
        height: msgContentTxt.height + 10;
        anchors.left: 0 == direction ? parent.left : headPortraitRect.right;
        anchors.leftMargin: (0 == direction) ? (parent.width - width - 60) : 4;
        anchors.top: nicknametxt.bottom;
        anchors.topMargin: 4;
        radius: 5;
        color: "#B0E0E6";
        border.color: "#00FF7F";
        border.width: 2;

        enabled: 0 == msgType ? true : false;
        visible: enabled;

        TextEdit
        {
            id: msgContentTxt;
            text: msgContent;
            color: "black";
            width: rootDelegateItem.width - 70;//parent.width;

            clip: true;
            font.family: "Consolas";
            font.pixelSize: 12;
            textFormat: Text.RichText;
            wrapMode: TextEdit.WordWrap;
            smooth: true;
            renderType: Text.NativeRendering;
            verticalAlignment: Text.AlignVCenter;
            anchors.verticalCenter: parent.verticalCenter;


        }
    }

    //图片消息框
    Rectangle
    {
        id: pictureRect;
        width: 64;
        height: 64;
        anchors.left: 0 == direction ? parent.left : headPortraitRect.right;
        anchors.leftMargin: (0 == direction) ? (parent.width - width - 60) : 4;
        anchors.top: nicknametxt.bottom;
        anchors.topMargin: 4;
        radius: 5;
        color: "#B0E0E6";
        border.color: "#00FF7F";
        border.width: 2;

        enabled: 1 == msgType ? true : false;
        visible: enabled;

        signal imageView(string imgFilePath);
        Image {
            id: pictureImg;
            source: pictureFilePath;
            anchors.centerIn: pictureRect;
            anchors.fill: pictureRect;
        }

        MouseArea
        {
            anchors.fill: pictureRect;
            onDoubleClicked: {
                var componentWindow = Qt.createComponent("test.qml");
                console.log("status", componentWindow.status);
                componentWindow.createObject
                componentWindow.show();
            }
/*            onDoubleClicked: {
                var str = 'import QtQuick 2.4;
               import QtQuick.Window 2.2;
               Window{
                   id: imageShowWindow;
                   height: %1;
                   width: %2;
                   flags: Qt.FramelessWindowHint;
                   color: "#708090";
                   Rectangle{
                       id: imageShowRect;
                       x: (parent.width - imageShow.width)/2;
                       y: (parent.height - imageShow.height)/2;
                       opacity: 1.0;
                       Image {
                           id: imageShow;
                           source: "%3";}
                   }

                   Rectangle
                   {
                       id: closeWindow;
                       x: imageShowWindow.width - 3 * closeText.paintedWidth;
                       y: 3 * closeText.height;
                       width: 32;
                       height: 32;
                       color: "red";
                       Text
                       {
                           id: closeText;
                           text: "X";
                       }
                       MouseArea{
                           anchors.fill: closeWindow;
                           onClicked: {
                               imageShowWindow.close();
                           }
                       }

                   }
               }
'.arg(parseInt(Screen.desktopAvailableHeight)).arg(parseInt(Screen.desktopAvailableWidth)).arg(pictureImg.source);
                console.log("str : ", str);
                var newObject = Qt.createQmlObject(str, rootRect, "errorfile");
                newObject.show();
            }
            */
        }
    }


    property bool pressed: false;
    property bool audioIsPlaying: false;
    property int duration: 3;
    property int currentImgIndex: -1;
    //語音圖標+語音長度文本
    Rectangle
    {
        id: audioRect;
        width: 108;
        height: 32;
        anchors.left: 0 == direction ? parent.left : headPortraitRect.right;
        anchors.leftMargin: (0 == direction) ? (parent.width - width - 60) : 4;
        anchors.top: nicknametxt.bottom;
        anchors.topMargin: 4;
        radius: 5;
        color: "#B0E0E6";
        border.color: "#00FF7F";
        border.width: 2;
        enabled: 2 == msgType ? true : false;
        visible: enabled;
        Image {
            id: audioImg;
            width: 32;
            height: 24;
            mirror: 0 == direction ? true : false;
            source: pressed ? "./res/ic_voice.png" : "./res/ic_voice_notListen.png";
            x: 0 == direction ? (audioRect.width - width) : 2;
            y: 4;
        }

        Text
        {
            id: audioLengthTxt;
            text: "%1\"".arg(audioPlayer.duration/1000);    //记录录音文件的长度，可以使用录音文件对象的duration直接获取
            x: 0 == direction ? (audioImg.x - width - 16) : audioImg.x + audioImg.width + 16;
            y: audioImg.y + (audioImg.height / 3);

        }

        Timer
        {
            id: aduioImgChangeTimer;
            running: false;
            interval: 500;
            repeat: true;

            onTriggered: {
                console.log("current audioImg.source:" + audioImg.source + "currentIndexImg:" + currentImgIndex + "duration:" + duration);

                if (0 == currentImgIndex)
                {
                    audioImg.source = "./res/ic_voice_1.png";
                    currentImgIndex = 1;
                }
                else if (1 == currentImgIndex)
                {
                    audioImg.source = "./res/ic_voice_pressed.png";
                    currentImgIndex = 2;
                }
                else if ((-1 == currentImgIndex) || (2 == currentImgIndex))
                {
                    audioImg.source = "./res/ic_voice_0.png";
                    currentImgIndex = 0;
                }
                duration -= interval;
                if (0 >= duration)
                {
                    audioImg.source = "./res/ic_voice_pressed.png";
                    currentImgIndex = 2;
                    running = false;
                }
            }
        }

        MediaPlayer
        {
            id: audioPlayer;
            source: audioFilePath;
        }

        MouseArea
        {
            anchors.fill: audioRect;
            hoverEnabled: true;
            onEntered: {
                cursorShape = Qt.PointingHandCursor;
            }
            onExited: {
                cursorShape = Qt.ArrowCursor;
            }

            onClicked:
            {
                //启动定时器，动态改变audioImg的图标
                //播放音频
                duration = audioPlayer.duration;
                if (!audioIsPlaying)
                {
                    aduioImgChangeTimer.start();
                    audioPlayer.play();
                    audioIsPlaying = true;
                }
                else
                {
                    aduioImgChangeTimer.stop();
                    audioImg.source = "res/ic_voice_pressed.png";
                    audioPlayer.pause();
                    audioIsPlaying = false;
                }
            }
        }

        Component.onCompleted: {
            duration = audioPlayer.duration;
            pressed = false;
            currentImgIndex = -1;
        }
    }


    //视频
//    property bool isVideoPlaying: false;
//    Rectangle
//    {

//        id: videoRect;
//        width: 96;
//        height: 96;
//        anchors.left: 0 == direction ? parent.left : headPortraitRect.right;
//        anchors.leftMargin: (0 == direction) ? (parent.width - width - 60) : 4;
//        anchors.top: nicknametxt.bottom;
//        anchors.topMargin: 4;
//        radius: 5;
//        color: "#B0E0E6";
//        border.color: "#00FF7F";
//        border.width: 2;

//        enabled: 3 == msgType ? true : false;
//        visible: enabled;

//        Rectangle
//        {
//            id: remindMsgRect;
//            border.color: "#F5DEB3";
//            border.width: 4;
//            anchors.verticalCenter:  videoRect.verticalCenter;
//            anchors.left: videoRect.left;
//            anchors.leftMargin: 12;
//            visible: false;
//            Text
//            {
//                id: remindMsg;
//                color: "#F4A460";
//                text: "点击播放视频";
//            }
//        }

//        MediaPlayer{
//            id: videoPlayer;
//            source: videoFilePath;

//            onPlaybackStateChanged:
//            {
//                console.log("current playbackState ", playbackState);
//                if(MediaPlayer.StoppedState == playbackState)
//                {
//                    isVideoPlaying = false;
////                    videoPlayer.destroy();
//                }
//            }
//        }

//        VideoOutput
//        {
//            id:videoOutput;
//            source: videoPlayer;
//            anchors.fill: parent;
//        }
//        MouseArea
//        {
//            anchors.fill: videoRect;
//            hoverEnabled: true;

//            onClicked: {
//                if (!isVideoPlaying)
//                {
//                    videoPlayer.play();
//                    isVideoPlaying = true;
//                }
//                else
//                {
//                    videoPlayer.pause();
//                    isVideoPlaying = false;
//                }
//            }

//            onEntered: {
//                if (!isVideoPlaying)
//                    remindMsgRect.visible = true;
//            }
//            onExited: {
//                if (!isVideoPlaying)
//                    remindMsgRect.visible = false;
//            }
//        }
//    }

    property bool isVideoPlaying: false;
    Rectangle
    {

        id: videoRect;
        width: 96;
        height: 96;
        anchors.left: 0 == direction ? parent.left : headPortraitRect.right;
        anchors.leftMargin: (0 == direction) ? (parent.width - width - 60) : 4;
        anchors.top: nicknametxt.bottom;
        anchors.topMargin: 4;
        radius: 5;
        color: "#B0E0E6";
        border.color: "#00FF7F";
        border.width: 2;

        enabled: 3 == msgType ? true : false;
        visible: enabled;

        Image
        {
            id: videoPlayerIconImg;
            width: 32;
            height: 32;
            source: "res/ic_video_player.png";
            anchors.horizontalCenter: videoRect.horizontalCenter;
            anchors.verticalCenter: videoRect.verticalCenter;
        }

        Rectangle
        {
            id: remindMsgRect;
            border.color: "#F5DEB3";
            border.width: 4;
            anchors.verticalCenter:  videoRect.verticalCenter;
            anchors.left: videoRect.left;
            anchors.leftMargin: 12;
            visible: false;
            Text
            {
                id: remindMsg;
//                color: "#F4A460";
//                text: "点击播放视频";
            }
        }

        MediaPlayer{
            id: videoPlayer;
            source: videoFilePath;

            onPlaybackStateChanged:
            {
                if (MediaPlayer.StoppedState == playbackState)
                {
                    isVideoPlaying = false;
                    videoRect.height = 96;
                    videoRect.width = 96;
                    videoPlayerIconImg.visible = true;
                }
            }
        }

        VideoOutput
        {
            id:videoOutput;
            source: videoPlayer;
            anchors.fill: parent;
            anchors.margins: 4;
        }
        MouseArea
        {
            anchors.fill: videoRect;
            hoverEnabled: true;
            onClicked: {
                if(!isVideoPlaying)
                {
                    videoPlayer.play();
                    isVideoPlaying = true;
                    videoPlayerIconImg.visible = false;
                    videoRect.width = 380;
                    videoRect.height = 300;
                }
                else
                {
                    videoPlayer.pause();
                    isVideoPlaying = false;
                    videoPlayerIconImg.visible = true;
                    videoRect.width = 96;
                    videoRect.height = 96;
                }
            }

            onEntered: {
                cursorShape = Qt.PointingHandCursor;
                if (!isVideoPlaying)
                    remindMsgRect.visible = true;
            }

            onExited: {
                cursorShape = Qt.ArrowCursor;
                if (!isVideoPlaying)
                    remindMsgRect.visible = false;
            }

        }
    }


}
