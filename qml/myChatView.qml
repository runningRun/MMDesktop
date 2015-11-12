import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2

Item {
    id: rootRect;
    width: 480;
    height: 560;
    objectName: "rootItem";

    ListModel{
        id:hisoryListViewModel;
        objectName: "historyListViewModel";
    }

    Rectangle
    {
        id: listViewRect;
        objectName: "listViewRect";
        anchors.top: parent.top;
        anchors.topMargin: 8;
        anchors.left: parent.left;
        anchors.leftMargin: 8;
        anchors.right: parent.right;
        anchors.rightMargin: 8;
        height: 3 * rootRect.height / 5;
        radius: 7;
        color: "#EEEEEE";
        border.color: "green";
        border.width: 2;

        ListView{
            id: historyListView;
            objectName: "historyListView";
            width: parent.width - 10;
            height: parent.height - 10;
            anchors.centerIn: parent;
            spacing: 6;
            clip: true;
            model: hisoryListViewModel;
            delegate: DelegateItemRect{
                senderName: md_senderName;
                direction: md_direction;
                msgType: md_msgtype;
                msgContent: md_msgContent;
                pictureFilePath: md_pictureFilePath;
                audioFilePath: md_audioFilePath;
                videoFilePath: md_videoFilePath;
            }
        }
    }

    //工具欄框
    Rectangle
    {
        id: toolbarRect;
        anchors.top: listViewRect.bottom;
        anchors.topMargin: 4;
        anchors.left: rootRect.left;
        anchors.leftMargin: 8;
        anchors.right: rootRect.right;
        anchors.rightMargin: 8;
        border.color: "lightgreen";
        border.width: 2;
        radius: 7;
        width: listViewRect.width;
        height: 32;

        Image {
            id: fontImage;
            source: "res/font.png";
            anchors.left: toolbarRect.left;
            anchors.leftMargin: 8;
            width: 24;
            height: 24;
            anchors.verticalCenter: toolbarRect.verticalCenter;
            MouseArea
            {
                anchors.fill: fontImage;
                hoverEnabled: true;
                onClicked: {
                    /*
                    fileType = 1;
                    defaultSelsectedNameFilter = "Images Files(*.jpg *.png *.gif)";
                    fileDialog.open();
                    msgDirection = 0;
                    */
                }
            }
        }

        Image {
            id: pictureImage;
            source: "res/picture.png";
            anchors.left: fontImage.right;
            anchors.leftMargin: 8;
            width: 24;
            height: 24;
            anchors.verticalCenter: toolbarRect.verticalCenter;
            MouseArea
            {
                anchors.fill: pictureImage;
                hoverEnabled: true;
                onClicked: {
                    fileType = 1;
                    defaultSelsectedNameFilter = "Images Files(*.jpg *.png *.gif)";
                    fileDialog.open();
                    msgDirection = 0;
                }
            }
        }

        Image {
            id: voiceImage;
            source: "res/voice.png";
            anchors.left: pictureImage.right;
            anchors.leftMargin: 8;
            width: 24;
            height: 24;
            anchors.verticalCenter: toolbarRect.verticalCenter;
            MouseArea
            {
                anchors.fill: voiceImage;
                hoverEnabled: true;
                onClicked: {
                    fileType = 2;
                    defaultSelsectedNameFilter = "Audio Files(*.mp3 *.wav)";
                    fileDialog.open();
                    msgDirection = 0;
                }
            }
        }

        Image {
            id: videoImage;
            source: "res/video.png";
            anchors.left: voiceImage.right;
            anchors.leftMargin: 8;
            width: 24;
            height: 24;
            anchors.verticalCenter: toolbarRect.verticalCenter;
            MouseArea
            {
                anchors.fill: videoImage;
                hoverEnabled: true;
                onClicked: {
                    fileType = 3;
                    defaultSelsectedNameFilter = "Video Files(*.mp4 *.avi *.wmv)";
                    fileDialog.open();
                    msgDirection = 0;
                }
            }
        }

    }

    Rectangle
    {
        id: textEditRect;
        anchors.top: toolbarRect.bottom;
        anchors.topMargin: 4;
        anchors.left: rootRect.left;
        anchors.leftMargin: 8;
        anchors.right: rootRect.right;
        anchors.rightMargin: 8;
        border.color: "lightgreen";
        border.width: 2;
        radius: 7;
        width: listViewRect.width;
        height: rootRect.height / 5;

        Flickable
        {
            id: flick
            width: parent.width-20
            height: parent.height-10
            anchors.centerIn: textEditRect
            contentWidth: edit.paintedWidth
            contentHeight: edit.paintedHeight
            clip: true

            function ensureVisible(r)
            {
                if (contentX >= r.x)
                    contentX = r.x;
                else if (contentX+width <= r.x+r.width)
                    contentX = r.x+r.width-width;
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY+height <= r.y+r.height)
                    contentY = r.y+r.height-height;
            }

            TextEdit
            {
                id: edit
                width: flick.width
                height: flick.height
                focus: true
                font.family: "Helvetica";
                font.pointSize: 12;
                wrapMode: TextEdit.Wrap
                onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
                Keys.onReturnPressed: {
                    edit.text = "";
                }
            }
        }
    }

    property var fileDialogTitle: "请选择一个文件";
    property var fileDialogNameFilters: "*.*";
    property var defaultSelsectedNameFilter: "All(*.*)";
    property int fileType: -1;
    property int msgDirection: -1;
    //打开文件对话框
    FileDialog
    {
        id: fileDialog;
        title: fileDialogTitle;
        nameFilters: ["Images Files(*.jpg *.png *.gif)",
                      "Audio Files(*.mp3 *.wav)",
                      "Video Files(*.mp4 *.avi *.wmv)",
                      "All(*.*)"];
        selectedNameFilter: defaultSelsectedNameFilter;
        onAccepted: {
            finishedPackage(agentID, fileType, msgDirection, fileDialog.fileUrl.toString());
        }

    }

    Rectangle
    {
        id: btnRect;
        anchors.left: rootRect.left;
        anchors.leftMargin: 8;
        anchors.top: textEditRect.bottom;
        anchors.topMargin: 4;
        anchors.right: rootRect.right;
        anchors.rightMargin: 8;
        anchors.bottom: rootRect.bottom;
        anchors.bottomMargin: 8;
        radius: 7;

        /*
        Rectangle
        {
            id: leftBtnRect;
            width: (btnRect.width / 2) - 8;
            height: btnRect.height;
            anchors.left: btnRect.left;
            anchors.rightMargin: 4;
            radius: 7;

            Button
            {
                id: leftSendTxtBtn;
                width: (leftBtnRect.width / 4) - 8;
                height: 0.6 * leftBtnRect.height;
                anchors.left: leftBtnRect.left;
                anchors.leftMargin: 4;
                anchors.top: leftBtnRect.top;
                anchors.topMargin: 10;
                text: "发送";

                onClicked: {
                    finishedPackage(0, 1, "");
                }
            }

            Button
            {
                id: leftSendPictureBtn;
                width: (leftBtnRect.width / 4) - 8;
                height: 0.6 * leftBtnRect.height;
                anchors.left: leftSendTxtBtn.right;
                anchors.leftMargin: 4;
                anchors.top: leftBtnRect.top;
                anchors.topMargin: 10;
                text: "图片";

                onClicked: {
                    fileType = 1;
                    defaultSelsectedNameFilter = "Images Files(*.jpg *.png *.gif)";
                    fileDialog.open();
                    msgDirection = 1;
                }
            }

            Button
            {
                id: leftSendVoiceBtn;
                width: (leftBtnRect.width / 4) - 8;
                height: 0.6 * leftBtnRect.height;
                anchors.left: leftSendPictureBtn.right;
                anchors.leftMargin: 4;
                anchors.top: leftBtnRect.top;
                anchors.topMargin: 10;
                text: "语音";
                onClicked: {
                    fileType = 2;
                    defaultSelsectedNameFilter = "Audio Files(*.mp3 *.wav)";
                    fileDialog.open();
                    msgDirection = 1;
                }
            }

            Button
            {
                id: leftSendVedioBtn;
                width: (leftBtnRect.width / 4) - 8;
                height: 0.6 * leftBtnRect.height;
                anchors.left: leftSendVoiceBtn.right;
                anchors.leftMargin: 4;
                anchors.top: leftBtnRect.top;
                anchors.topMargin: 10;
                text: "视频";

                onClicked: {
                    fileType = 3;
                    defaultSelsectedNameFilter = "Video Files(*.mp4 *.avi *.wmv)";
                    fileDialog.open();
                    msgDirection = 1;
                }
            }
        }
        */

        Rectangle
        {
            id: rightBtnRect;
            width: (btnRect.width / 2) - 8;
            height: btnRect.height;
            anchors.right: btnRect.right;
            anchors.leftMargin: 4;
            radius: 7;

            Button
            {
                id: rightSendTxtBtn;
                width: (rightBtnRect.width / 4) - 8;
                height: 0.6 * rightBtnRect.height;
                anchors.left: rightBtnRect.left;
                anchors.leftMargin: 4;
                anchors.top: rightBtnRect.top;
                anchors.topMargin: 10;
                text: "发送";
                onClicked: {
                    finishedPackage(agentID, 0, 0, edit.text);
                }
            }/*

            Button
            {
                id: rightSendPictureBtn;
                width: (rightBtnRect.width / 4) - 8;
                height: 0.6 * rightBtnRect.height;
                anchors.left: rightSendTxtBtn.right;
                anchors.leftMargin: 4;
                anchors.top: rightBtnRect.top;
                anchors.topMargin: 10;
                text: "图片";

                onClicked: {
                    fileType = 1;
                    defaultSelsectedNameFilter = "Images Files(*.jpg *.png *.gif)";
                    fileDialog.open();
                    msgDirection = 0;
                }
            }

            Button
            {
                id: rightSendVoiceBtn;
                width: (rightBtnRect.width / 4) - 8;
                height: 0.6 * rightBtnRect.height;
                anchors.left: rightSendPictureBtn.right;
                anchors.leftMargin: 4;
                anchors.top: rightBtnRect.top;
                anchors.topMargin: 10;
                text: "语音";

                onClicked: {
                    fileType = 2;
                    defaultSelsectedNameFilter = "Audio Files(*.mp3 *.wav)";
                    fileDialog.open();
                    msgDirection = 0;
                }
            }

            Button
            {
                id: rightSendVedioBtn;
                width: (rightBtnRect.width / 4) - 8;
                height: 0.6 * rightBtnRect.height;
                anchors.left: rightSendVoiceBtn.right;
                anchors.leftMargin: 4;
                anchors.top: rightBtnRect.top;
                anchors.topMargin: 10;
                text: "视频";

                onClicked:
                {
                    fileType = 3;
                    defaultSelsectedNameFilter = "Video Files(*.mp4 *.avi *.wmv)";
                    fileDialog.open();
                    msgDirection = 0;
                }
            }*/

            Button
            {
                id: endSession;
                width: (rightBtnRect.width / 4) - 8;
                height: 0.6 * rightBtnRect.height;
                anchors.right: rightBtnRect.right;
                anchors.rightMargin: 10;
                anchors.top: rightBtnRect.top;
                anchors.topMargin: 10;
                text: "結束";

                onClicked:
                {
                    sessionHandle.finishSession(sessionID);
                }
            }
        }

    }

    function finishedPackage(senderName, msgtype, direction, content)
    {
        addNewItem2ListView(senderName, msgtype, direction, content);
        sessionHandle.sendInfo(sessionID, msgtype, content);
    }

    function addNewItem2ListView(senderName, msgtype, direction, content)
    {
        switch (msgtype)
        {
        case 0:             //文本
            historyListView.model.append({
                        "md_senderName": senderName,
                        "md_msgtype": msgtype,
                        "md_direction": direction,
                        "md_msgContent": content,
                        "md_pictureFilePath": "",
                        "md_audioFilePath": "",
                        "md_videoFilePath": ""
                    });
            edit.text = "";
            break;
        case 1:             //图片
            historyListView.model.append({
                        "md_senderName": senderName,
                        "md_msgtype": msgtype,
                        "md_direction": direction,
                        "md_msgContent": "",
                        "md_pictureFilePath": content,
                        "md_audioFilePath": "",
                        "md_videoFilePath": ""
                    });
            edit.text = "";
            break;
        case 2:             //語音
            historyListView.model.append({
                        "md_senderName": senderName,
                        "md_msgtype": msgtype,
                        "md_direction": direction,
                        "md_msgContent": "",
                        "md_pictureFilePath": "",
                        "md_audioFilePath": content,
                        "md_videoFilePath": ""
                    });
            edit.text = "";
            break;
        case 3:             //视频
            historyListView.model.append({
                        "md_senderName": senderName,
                        "md_msgtype": msgtype,
                        "md_direction": direction,
                        "md_msgContent": "",
                        "md_pictureFilePath": "",
                        "md_audioFilePath": "",
                        "md_videoFilePath": content
                    });
            edit.text = "";
            break;
        }
        historyListView.positionViewAtEnd();
    }

    Connections
    {
        target: sessionHandle;
        onAddNewItem:{
            console.log("onAddNewItem, ", newItemContent);
            addNewItem2ListView(senderName, newItemMsgtype, newItemDirection, newItemContent);
        }
    }
}

