#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serial.h"
#include <QJoysticks.h>
#include <QTimer>
#include <iostream>
#include <math.h>
#include <QString>

namespace Ui {
class MainWindow;
}

enum joyButtons{
    triangulo,
    bolinha,
    xis,
    quadrado,
    l_,
    r_,
    l2_,
    r2_,
    select_,
    start,
    leftClick,
    rightClick
};

enum joyAxis{
    leftX,
    leftY
};

enum joyPOVs{
    up,
    right_,
    down,
    left_
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    float ajusteVR10FRENTE(float);
    float ajusteVR10RE(float);
    float ajusteMotor(float,int);

private Q_SLOTS:
  void updateVels();
  void control1();
  void control2();
  void control3();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QJoysticks* instance;
    Serial serial;
    QTimer *timer1, *timer2, *timer3, *timerUpVels;
    clock_t clockStart;
    int startStatus1, startStatus2, startStatus3 ;
    int joy1ID, joy2ID, joy3ID;
    float vMin, vMax, vMin1, vMax1, vMin2, vMax2, vMin3, vMax3;
    float v1, v2, v3;
    float vLeft1, vRight1, vLeft2, vRight2, vLeft3, vRight3;
    float direction1, direction2 , direction3;
    int directionPOV1, directionPOV2, directionPOV3;
    float vTurn, vGiro;
    int robo1, robo2, robo3;
};

#endif // MAINWINDOW_H
