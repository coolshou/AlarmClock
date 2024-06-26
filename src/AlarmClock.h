#pragma once
//#pragma execution_character_set("utf-8")

#include <QtGui>
#include <QtWidgets>
#include "ui_AlarmClock.h"
#include <QPushButton>
#include "AlertTimeDlg.h"
#include <QtMultimedia/QMediaPlayer>
#include <QVideoWidget>

class AlarmClock : public QDialog
{
    Q_OBJECT

public:
    AlarmClock(QWidget *parent = Q_NULLPTR);
    virtual void paintEvent(QPaintEvent *event);

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);

    //定時器
    virtual void timerEvent(QTimerEvent *event);
    int timerId;

    QPushButton* closeButton;
    QPushButton* minButton;
    QPushButton* aboutButton;

private:
    QPixmap m_titleImage;

    bool m_dragging; // 是否正在拖動
    QPoint m_startPosition; // 拖動開始前的鼠標位置
    QPoint m_framePosition; // 窗體的原始位置

    QSystemTrayIcon m_systray;
    AlertTimeDlg* alertTimeDlg;
    AlertTimeDlg* alertTimeDlg2;

    QMediaPlayer *player;
    QMediaPlayer *player2;
    bool bSystray = false;

    //鬧鈴1
    bool bRuningClock1;
    int clock1HH;
    int clock1MM;


    //鬧鈴2
    bool bRuningClock2;
    int clock2HH;
    int clock2MM;

private slots:
    void OnBtnClose();
    void OnBtnMin();
    void OnBtnAbout();
    void OnExit();
    int  OnSystemTrayClicked(QSystemTrayIcon::ActivationReason reason);
    void OnBtnStartClock1();
    void OnBtnStartClock2();
    void OnListen1();
    void OnListen2();
    void OnCloseAlertTimeDlg1();
    void OnCloseAlertTimeDlg2();


private:
    Ui::AlarmClockClass ui;
};
