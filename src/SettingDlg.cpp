#include "SettingDlg.h"
#include <QDebug>

SettingDlg::SettingDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    //去掉?
    Qt::WindowFlags flags = Qt::Dialog; flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    ui.stackedWidget->setCurrentIndex(1);
    ui.label1->setText("作者：蝸牛201\r\n版本：1.1.0\r\n發佈日期：2019/04/24\r\n微信公號：碼農code之路");

    this->setWindowTitle("設置");

    connect(ui.btnSetting, &QToolButton::clicked, this, &SettingDlg::OnBaseSetting);
    connect(ui.btnAbout, &QToolButton::clicked, this, &SettingDlg::OnAbout);

    connect(ui.checkBox, &QCheckBox::stateChanged, this, &SettingDlg::OnCheckBox);

//	if (IsAutoStart())
//	{
//		ui.checkBox->setChecked(true);
//	}
//	else
//	{
//		ui.checkBox->setChecked(false);
//	}
}

SettingDlg::~SettingDlg()
{
}

void SettingDlg::OnBaseSetting()
{
    ui.stackedWidget->setCurrentIndex(0);
}

void SettingDlg::OnAbout()
{
    ui.stackedWidget->setCurrentIndex(1);
}


void SettingDlg::OnCheckBox(int state)
{
    qDebug() << state;
//    if (state == 0) //取消選中
//    {
//        DelAutoStart();
//    }
//    else if (state == 2) //選中
//    {
//        SetAutoStart();
//    }
}
