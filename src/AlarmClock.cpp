#include "AlarmClock.h"
#include <QPainter>
#include "AlertTimeDlg.h"
#include <QTime>
#include <QMessageBox>
#include "AboutDlg.h"
#include "NowTimeDlg.h"
#include "SettingDlg.h"

AlarmClock::AlarmClock(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    //不顯示標題欄（無邊框）
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setAttribute(Qt::WA_TranslucentBackground);
    m_titleImage.load(":/AlarmClock/Resources/background.jpg");

//系統托盤
    //設置提示文字
    m_systray.setToolTip("蝸牛鬧鈴");
    // 設置托盤圖標
    m_systray.setIcon(QIcon(":/AlarmClock/Resources/clock.png"));
    //托盤菜單項
    QMenu * menu = new QMenu();
    menu->addAction(ui.actionExit);
    m_systray.setContextMenu(menu);
    //顯示托盤
    m_systray.show();

    m_systray.show();
//主頁面關閉按鈕，最下化按鈕
    closeButton = new QPushButton(this);
    //closeButton->setIcon(QIcon(":/AlarmClock/Resources/close_normal.png"));
    closeButton->setGeometry(this->rect().right() - 45, 10, 40, 30);
    //closeButton->setFlat(true);
    closeButton->setStyleSheet("QPushButton{background-color:transparent; border-image: url(:/AlarmClock/Resources/close.png);}"
    "QPushButton:hover{background-color:rgb(6,168,240); border-image: url(:/AlarmClock/Resources/close.png);}"
    "QPushButton:pressed{background-color: rgb(6,168,220); border-image: url(:/AlarmClock/Resources/close.png);}");

    minButton = new QPushButton(this);
    minButton->setGeometry(this->rect().right() - 85, 10, 40, 30);
    minButton->setStyleSheet("QPushButton{background-color:transparent; border-image: url(:/AlarmClock/Resources/min.png);}"
        "QPushButton:hover{background-color:rgb(6,168,240); border-image: url(:/AlarmClock/Resources/min.png);}"
        "QPushButton:pressed{background-color: rgb(6,168,220); border-image: url(:/AlarmClock/Resources/min.png);}");

    aboutButton = new QPushButton(this);
    aboutButton->setGeometry(this->rect().right() - 115, 10, 30, 30);
    aboutButton->setStyleSheet("QPushButton{background-color:transparent; border-image: url(:/AlarmClock/Resources/setting.png);}"
        "QPushButton:hover{background-color:rgb(6,168,240); border-image: url(:/AlarmClock/Resources/setting.png);}"
        "QPushButton:pressed{background-color: rgb(6,168,220); border-image: url(:/AlarmClock/Resources/setting.png);}");

    //labelTime顯示時間
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd  hh:mm:ss");
    ui.labelTime->setText(strDateTime);

    //開始定時器
    timerId = startTimer(1000);

    //Tab1
    ui.btnStart1Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
        "QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
        "QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
    ui.btnStart2Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
        "QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
        "QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");


    //初始化變量
    player = NULL;
    player2 = NULL;
    alertTimeDlg = NULL;
    alertTimeDlg2 = NULL;
    bRuningClock1 = false;
    bRuningClock2 = false;
    int clock1HH = 0;
    int clock1MM = 0;
    int clock2HH = 0;
    int clock2MM = 0;


    //綁定響應事件
    connect(minButton, SIGNAL(clicked()), this, SLOT(OnBtnMin()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(OnBtnClose()));
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(OnBtnAbout()));
    connect(&m_systray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(OnSystemTrayClicked(QSystemTrayIcon::ActivationReason)));
    connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(OnExit()));

    connect(ui.btnStart1Clock, &QPushButton::clicked, this, &AlarmClock::OnBtnStartClock1);
    connect(ui.btnStart2Clock, &QPushButton::clicked, this, &AlarmClock::OnBtnStartClock2);

    connect(ui.listenBtn1, &QPushButton::clicked, this, &AlarmClock::OnListen1);
    connect(ui.listenBtn2, &QPushButton::clicked, this, &AlarmClock::OnListen2);
}


void AlarmClock::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect frameRect = rect();
    frameRect.adjust(1, 1, -1, -1);

    this->resize(452, 549);

    //畫背景和邊框
    painter.setPen(QColor(0x66, 0x66, 0x66));
    painter.setBrush(QColor(0xFF, 0xFF, 0xFF));
    painter.drawRoundRect(frameRect, 2, 2);


    //畫標題欄
    QRect titleRect = frameRect;
    titleRect.setBottom(frameRect.top() + 80);

    //添加標題欄背景圖後設置圓角
    QPainterPath path;
    path.addRoundRect(frameRect.adjusted(1, 1, 0, -1), 2, 2);
    path.intersects(titleRect);
    painter.setClipPath(path);

    painter.drawPixmap(titleRect, m_titleImage);
}



/************************************
@ Brief:		鼠標按下響應函數
@ Author:		woniu201
@ Created:		2019/04/16
@ Return:
************************************/
void AlarmClock::mousePressEvent(QMouseEvent *event)
{
    // 只響應左鍵
    if (event->button() == Qt::LeftButton)
    {
        QRect titleRect = rect();
        titleRect.setBottom(titleRect.top() + 80);

        if (titleRect.contains(event->pos()))
        {
            m_dragging = true;
            m_startPosition = event->globalPos();
            m_framePosition = frameGeometry().topLeft();
        }
    }

    QWidget::mousePressEvent(event);
}

/************************************
@ Brief:		鼠標移動響應函數
@ Author:		woniu201
@ Created:		2019/04/16
@ Return:
************************************/
void AlarmClock::mouseMoveEvent(QMouseEvent *event)
{
    // 只響應左鍵
    if (event->buttons() & Qt::LeftButton)
    {
        if (m_dragging)
        {
            // delta 相對偏移量,
            QPoint delta = event->globalPos() - m_startPosition;

            // 新位置：窗體原始位置  + 偏移量
            move(m_framePosition + delta);
        }
    }

    QWidget::mouseMoveEvent(event);
}

/************************************
@ Brief:		鼠標釋放響應函數
@ Author:		woniu201
@ Created:		2019/04/16
@ Return:
************************************/
void AlarmClock::mouseReleaseEvent(QMouseEvent * event)
{
    m_dragging = false;
    QWidget::mouseReleaseEvent(event);
}



/************************************
@ Brief:		定時器
@ Author:		woniu201
@ Created:		2019/04/16
@ Return:
************************************/
void AlarmClock::timerEvent(QTimerEvent *event)
{
    ui.labelTime->setText("");
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd  hh:mm:ss");
    ui.labelTime->setText(strDateTime);

    if (bRuningClock1)
    {
        QTime sysTime = QTime::currentTime();
        if ((sysTime.hour()== clock1HH) && (sysTime.minute()== clock1MM))
        {
            if (alertTimeDlg==NULL)
            {
                alertTimeDlg = new AlertTimeDlg(this, "./Resources/sound/sound1.mp3", "鬧鈴1：" + ui.textEdit1->toPlainText());
                QSize size = qApp->screens()[0]->size();
                int width = size.width();
                int height =size.height();
//				int width = GetSystemMetrics(SM_CXFULLSCREEN);
//				int height = GetSystemMetrics(SM_CYFULLSCREEN);
                alertTimeDlg->move(width - alertTimeDlg->width(), height - alertTimeDlg->height());
                if (!bSystray)
                {
                    this->setWindowFlags(Qt::WindowStaysOnTopHint);//托盤情況下不生效，直接show可顯示，最小化情況下需要設置該項
                }
                alertTimeDlg->show();

                connect(alertTimeDlg, &AlertTimeDlg::closeSignal, this, &AlarmClock::OnCloseAlertTimeDlg1);
                alertTimeDlg->setWindowTitle("鬧鈴中");
                ui.labelCountDown1->setText("時間到了！");

            }

        }
        else
        {
            //鬧鈴倒計時
            QTime endTime;
            endTime.setHMS(clock1HH, clock1MM, 0);
            qDebug() << endTime.toString("hh:mm ss");

            QTime startTime = QTime::currentTime();
            int sec = startTime.secsTo(endTime);

            int hh = int(sec / 3600);
            int mm = int((sec - hh * 3600) / 60);
            int ss = sec - hh * 3600 - mm * 60;

            ui.labelCountDown1->setText(QString::asprintf("倒計時：%02d:%02d %02d", hh, mm, ss));
        }
    }

    if (bRuningClock2)
    {
        QTime sysTime = QTime::currentTime();
        if ((sysTime.hour() == clock2HH) && (sysTime.minute() == clock2MM))
        {
            if (alertTimeDlg2 == NULL)
            {
                alertTimeDlg2 = new AlertTimeDlg(this, "./Resources/sound/sound1.mp3", "鬧鈴2：" + ui.textEdit2->toPlainText());
                QSize size = qApp->screens()[0]->size();
                int width = size.width();
                int height =size.height();
//                int width = GetSystemMetrics(SM_CXFULLSCREEN);
//                int height = GetSystemMetrics(SM_CYFULLSCREEN);
                alertTimeDlg2->move(width - alertTimeDlg2->width(), height - alertTimeDlg2->height());
                if (!bSystray)
                {
                    this->setWindowFlags(Qt::WindowStaysOnTopHint);
                }
                alertTimeDlg2->show();
                connect(alertTimeDlg2, &AlertTimeDlg::closeSignal, this, &AlarmClock::OnCloseAlertTimeDlg2);

                alertTimeDlg2->setWindowTitle("鬧鈴中");
                ui.labelCountDown2->setText("時間到了！");
            }

        }
        else
        {
            //鬧鈴倒計時
            QTime endTime;
            endTime.setHMS(clock2HH, clock2MM, 0);

            QTime startTime = QTime::currentTime();
            int sec = startTime.secsTo(endTime);

            int hh = int(sec / 3600);
            int mm = int((sec - hh * 3600) / 60);
            int ss = sec - hh * 3600 - mm * 60;

            ui.labelCountDown2->setText(QString::asprintf("倒計時：%02d:%02d %02d", hh, mm, ss));
        }
    }

    //整點報時
    if (QTime::currentTime().minute()==0 && QTime::currentTime().second()==0)
    {
        NowTimeDlg* nowTimeDlg = new NowTimeDlg;
        nowTimeDlg->show();
    }
}

/************************************
@ Brief:		關閉主窗口
@ Author:		woniu201
@ Created: 		2019/04/16
@ Return:
************************************/
void AlarmClock::OnBtnClose()
{
    this->hide();
    bSystray = true;
}

/************************************
@ Brief:		最小化主窗口
@ Author:		woniu201
@ Created: 		2019/04/16
@ Return:
************************************/
void AlarmClock::OnBtnMin()
{
    this->showMinimized();
}

/************************************
@ Brief:		關於
@ Author:		woniu201
@ Created:		2019/04/24
@ Return:
************************************/
void AlarmClock::OnBtnAbout()
{
// 	AboutDlg aboutDlg(this);
// 	aboutDlg.setModal(true);
// 	aboutDlg.setWindowTitle("關於");
// 	aboutDlg.ui.label1->setText("作者：蝸牛201\r\n版本：1.1.0\r\n發佈日期：2019/04/24\r\n微信公號：碼農code之路");
// 	aboutDlg.exec();

    SettingDlg settingDlg;
    settingDlg.exec();
}

/************************************
@ Brief:		托盤退出按鈕
@ Author:		woniu201
@ Created:		2019/04/17
@ Return:
************************************/
void AlarmClock::OnExit()
{
    QApplication::exit(0);
}

/************************************
@ Brief:		雙擊托盤
@ Author:		woniu201
@ Created:		2019/04/17
@ Return:
************************************/
int  AlarmClock::OnSystemTrayClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger
        || reason == QSystemTrayIcon::DoubleClick)
    {
        //不顯示標題欄（無邊框）
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

        // 顯示主窗口
        this->showNormal();
        bSystray = false;
    }
    return 0;
}

/************************************
@ Brief:		鬧鈴1試聽
@ Author:		woniu201
@ Created: 		2019/04/18
@ Return:
************************************/
void AlarmClock::OnListen1()
{	//player->setMedia(QUrl::fromLocalFile(":/AlarmClock/Resources/sound/鈴聲1.mp3"));
    //player->setMedia(QUrl(":/AlarmClock/Resources/sound/sound1.mp3"));
    //player->setMedia(QMediaContent(QUrl::fromLocalFile("./Resources/sound/sound1.mp3")));
    if (player)
    {
        delete player;
        player = NULL;
    }
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("./Resources/sound/sound1.mp3"));
    player->play();
}

/************************************
@ Brief:		鬧鈴2試聽
@ Author:		woniu201
@ Created:		2019/04/24
@ Return:
************************************/
void AlarmClock::OnListen2()
{
    if (player2)
    {
        delete player2;
        player2 = NULL;
    }
    player2 = new QMediaPlayer;
    player2->setMedia(QUrl::fromLocalFile("./Resources/sound/sound1.mp3"));
    player2->play();
}

/************************************
@ Brief:		關閉鬧鈴1
@ Author:		woniu201
@ Created:		2019/04/24
@ Return:
************************************/
void AlarmClock::OnCloseAlertTimeDlg1()
{
    //不顯示標題欄（無邊框）
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->showNormal();
    bSystray = false;
    bRuningClock1 = false;
    ui.btnStart1Clock->setText("開啟鬧鈴");
    ui.btnStart1Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
        "QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
        "QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
    ui.comboBoxHH1->setEnabled(true);
    ui.comboBoxMM1->setEnabled(true);
    ui.comboBoxRing1->setEnabled(true);
    ui.textEdit1->setEnabled(true);
    ui.labelCountDown1->setText("倒計時！");
    alertTimeDlg = NULL;
}

/************************************
@ Brief:		關閉鬧鈴2
@ Author:		woniu201
@ Created:		2019/04/24
@ Return:
************************************/
void AlarmClock::OnCloseAlertTimeDlg2()
{
    //不顯示標題欄（無邊框）
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->showNormal();
    bRuningClock2 = false;
    ui.btnStart2Clock->setText("開啟鬧鈴");
    ui.btnStart2Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
        "QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
        "QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
    ui.comboBoxHH2->setEnabled(true);
    ui.comboBoxMM2->setEnabled(true);
    ui.comboBoxRing2->setEnabled(true);
    ui.textEdit2->setEnabled(true);
    ui.labelCountDown2->setText("倒計時！");
    alertTimeDlg2 = NULL;
}


/************************************
@ Brief:		開啟/關閉鬧鈴1
@ Author:		woniu201
@ Created: 		2019/04/16
@ Return:
************************************/
void AlarmClock::OnBtnStartClock1()
{
// 	NowTimeDlg* nowTimeDlg = new NowTimeDlg;
// 	nowTimeDlg->show();
// 	return;
    if (bRuningClock1)
    {
        bRuningClock1 = false;
        ui.btnStart1Clock->setText("開啟鬧鈴");
        ui.btnStart1Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
            "QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
            "QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
        ui.comboBoxHH1->setEnabled(true);
        ui.comboBoxMM1->setEnabled(true);
        ui.comboBoxRing1->setEnabled(true);
        ui.textEdit1->setEnabled(true);
        ui.labelCountDown1->setText("倒計時！");
    }
    else
    {
        // 		alertTimeDlg = new AlertTimeDlg(this, "./Resources/sound/sound1.mp3", "鬧鈴1：時間到了！");
        //
        // 		int width = GetSystemMetrics(SM_CXFULLSCREEN);
        // 		int height = GetSystemMetrics(SM_CYFULLSCREEN);
        // 		alertTimeDlg->move(width - alertTimeDlg->width(), height - alertTimeDlg->height());
        // 		alertTimeDlg->show();
        // 		connect(alertTimeDlg, &AlertTimeDlg::closeSignal, this, &AlarmClock::OnCloseAlertTimeDlg1);
        // 		alertTimeDlg->setWindowTitle("鬧鈴中");

        clock1HH = ui.comboBoxHH1->currentText().toInt();
        clock1MM = ui.comboBoxMM1->currentText().toInt();

        QTime curTime = QTime::currentTime();
        //if ((clock1HH*3600 +clock1MM*60) >= (curTime.hour()*3600+ curTime.minute()*60+curTime.second()))
        if ((clock1HH * 3600 + clock1MM * 60) >= (curTime.hour() * 3600 + curTime.minute() * 60))
        {
            bRuningClock1 = true;
            ui.btnStart1Clock->setText("鬧鈴中");
            ui.btnStart1Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:red; font-size:30px; border-radius:10px;padding:2px 4px;}"
                "QPushButton:hover{background-color: rgb(6,168,240); color:red; border-radius:10px;padding:2px 4px;}"
                "QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:red; border-radius:10px;padding:2px 4px;}");
            ui.comboBoxHH1->setEnabled(false);
            ui.comboBoxMM1->setEnabled(false);
            ui.comboBoxRing1->setEnabled(false);
            ui.textEdit1->setEnabled(false);
        }
        else
        {
            QMessageBox::about(NULL, "提示", "<font color='red'>鬧鈴時間錯誤！</font>" );
        }
    }
}

/************************************
@ Brief:		開啟/關閉鬧鈴2
@ Author:		woniu201
@ Created:		2019/04/23
@ Return:
************************************/
void AlarmClock::OnBtnStartClock2()
{
    if (bRuningClock2)
    {
        bRuningClock2 = false;
        ui.btnStart2Clock->setText("開啟鬧鈴");
        ui.btnStart2Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
            "QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
            "QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
        ui.comboBoxHH2->setEnabled(true);
        ui.comboBoxMM2->setEnabled(true);
        ui.comboBoxRing2->setEnabled(true);
        ui.textEdit2->setEnabled(true);
        ui.labelCountDown2->setText("倒計時！");
    }
    else
    {
        clock2HH = ui.comboBoxHH2->currentText().toInt();
        clock2MM = ui.comboBoxMM2->currentText().toInt();

        QTime curTime = QTime::currentTime();
        if ((clock2HH * 3600 + clock2MM * 60) >= (curTime.hour() * 3600 + curTime.minute() * 60 + curTime.second()))
        {
            bRuningClock2 = true;
            ui.btnStart2Clock->setText("鬧鈴中");
            ui.btnStart2Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:red; font-size:30px; border-radius:10px;padding:2px 4px;}"
                "QPushButton:hover{background-color: rgb(6,168,240); color:red; border-radius:10px;padding:2px 4px;}"
                "QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:red; border-radius:10px;padding:2px 4px;}");
            ui.comboBoxHH2->setEnabled(false);
            ui.comboBoxMM2->setEnabled(false);
            ui.comboBoxRing2->setEnabled(false);
            ui.textEdit2->setEnabled(false);
        }
        else
        {
            QMessageBox::about(NULL, "提示", "<font color='red'>鬧鈴時間錯誤！</font>");
        }
    }
}
