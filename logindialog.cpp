#include "logindialog.h"
//#include "ui_dialog.h"
#include "ui_logindialog.h"
#include "globalxagentocx.h"
#include <QMessageBox>
#include "agentinfo.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    QStringList texts;
    texts.append("PhoneBank");
    texts.append("CreditCard");
    this->ui->comboBoxAgentGroup->addItems(texts);
    this->ui->axWidget_AgentOCX->hide();

    GlobalXAgentOCX::globalXAgentOCX = this->ui->axWidget_AgentOCX;

    connect(this->ui->pushButtonLogin, SIGNAL(clicked()), this, SLOT(onLoginButtonClicked()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onLoginButtonClicked()
{
    QString agentid = this->ui->lineEditAgentID->text();
    QString agentName = this->ui->lineEditAgentName->text();
    QString thisDN = this->ui->lineEditThisDN->text();
    QString agentGroup = this->ui->comboBoxAgentGroup->currentText();

    if ((agentid.length() == 0) | (agentName.length() == 0) | (agentGroup.length() == 0) | (thisDN.length() == 0))
    {
        QMessageBox msgBox;
        msgBox.setText("请填写必要信息");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QString agentinfo = "";
    agentinfo += ("agentid=" +agentid + "|");
    agentinfo += ("thisdn=" + thisDN+ "|");
    agentinfo += ("agenttype=1|tenantid=1234|");
    agentinfo += ("groupid=" + agentGroup + "|");
    agentinfo += ("agentname=" + agentName);

    AgentInfo::agentGroupID = agentGroup;
    AgentInfo::agentID = agentid;
    AgentInfo::agentName = agentName;
    AgentInfo::agentThisDN = thisDN;

    GlobalXAgentOCX* agentOCX = GlobalXAgentOCX::instance();
    agentOCX->setAgentInfo(agentinfo);
    agentOCX->startUp();

    emit btnLoginClicked();
}
