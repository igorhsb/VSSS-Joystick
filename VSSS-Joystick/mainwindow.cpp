#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    joy1ID(0),
    joy2ID(1),
    joy3ID(2),
    vMax(80),
    vMin(0)
{
    ui->setupUi(this);
    instance = QJoysticks::getInstance();
    instance->setVirtualJoystickRange (1);
    instance->setVirtualJoystickEnabled (false);
    ui->joyCbx->addItems(instance->deviceNames());

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos){
        if(info.portName() != "ttyS0")
            ui->serialCbx->addItem(info.portName());
    }

    timer1 = new QTimer();
    connect(timer1, SIGNAL(timeout()), this, SLOT(control1()));
    timer1->stop();
    startStatus1 = false;

    timer2 = new QTimer();
    connect(timer2, SIGNAL(timeout()), this, SLOT(control2()));
    timer2->stop();
    startStatus2 = false;

    timer3 = new QTimer();
    connect(timer3, SIGNAL(timeout()), this, SLOT(control3()));
    timer3->stop();
    startStatus3 = false;

    timerUpVels = new QTimer();
    connect(timerUpVels, SIGNAL(timeout()), this, SLOT(updateVels()));
    timerUpVels->stop();

    vGiro = 70;
    vMax1 = vMax;
    vMin1 = vMin;
    vMax2 = vMax;
    vMin2 = vMin;
    vMax3 = vMax;
    vMin3 = vMin;
    v1 = (vMax1+vMin1)/2;
    v2 = (vMax2+vMin2)/2;
    v3 = (vMax3+vMin3)/2;
    ui->lcdNumber->setPalette(Qt::green);
    ui->lcdNumber_2->setPalette(Qt::red);
    ui->lcdNumber_3->setPalette(Qt::blue);
}

MainWindow::~MainWindow()
{
    serial.close();
    timer1->stop();
    timer2->stop();
    timer3->stop();
    timerUpVels->stop();
    delete ui;

}


void MainWindow::on_pushButton_clicked()
{
    //  cout << "BTN: " << endl;
    if(ui->pushButton->text() == "Start")
    {
        if(serial.connectSerial(ui->serialCbx->currentText()))
        {
            ui->pushButton->setText("Stop");
            clockStart = clock();
            timer1->start(10);
            timer2->start(10);
            timer3->start(10);
            timerUpVels->start(10);
        }
    }
    else
    {
        serial.close();
        ui->pushButton->setText("Start");
        timer1->stop();
        timer2->stop();
        timer3->stop();
        timerUpVels->stop();
    }
}

void MainWindow::updateVels()
{
    robo1 = ui->roboSpinBx->value();
    robo2 = ui->roboSpinBx2->value();
    robo3 = ui->roboSpinBx3->value();
    int l1, l2, l3, r1, r2, r3;
    l1 = ui->lr1SPX->value()/2;
    l2 = ui->lr2SPX->value()/2;
    l3 = ui->lr2SPX->value()/2;
    r1 = ui->rr1SPX->value()/2;
    r2 = ui->rr2SPX->value()/2;
    r3 = ui->rr3SPX->value()/2;

    if(startStatus1)
    {
        if(v1 > 0){
            //cout << "Right frente: " << ajusteVR10FRENTE(vRight1)<< endl ;
            cout << "Left frente" << left << endl ;

            //serial.sendMsg(robo1,vLeft1 + l1, ajusteMotor(vRight1,10));
            serial.sendMsg(robo1,vLeft1, vRight1);
            //            serial.sendMsg(robo1, ajusteVR13(vLeft1),vRight1);
            //cout << "Left: " << ajusteVR13(vLeft1) << endl;

        }
        else{
            cout << "Right re: " << ajusteVR10RE(vRight1) << endl;
            serial.sendMsg(robo1,vLeft1 + l1, vRight1/*vRight1 + r1*/);
        }
        cout << "Left re: " << left << endl;

        //serial.sendMsg(robo1,vLeft1 + l1, ajusteVR10RE(vRight1)/*vRight1 + r1*/);
    }
    else
    {
        serial.sendMsg(robo1,0,0);
        //                cout << "Vel: 0" << endl ;
    }

    if(startStatus2)
    {
        serial.sendMsg(robo2,vLeft2 + l2,vRight2 + r2);
    }
    else
    {
        serial.sendMsg(robo2,0,0);
    }

    if(startStatus3)
    {
        serial.sendMsg(robo3,vLeft3 + l3,vRight3 + r3);
    }
    else
    {
        serial.sendMsg(robo3,0,0);
    }
    ui->lcdNumber->display(v1);
    ui->lcdNumber_2->display(v2);
    ui->lcdNumber_3->display(v3);
}

void MainWindow::control1()
{
    if(instance->getNumAxes(joy1ID) > 0)
        direction1 = instance->getAxis(joy1ID,leftX);
    else
        direction1 = 0;

    if(instance->getNumPOVs(joy1ID) > 0)
    {
        if(instance->getPOV(joy1ID,0) == 90)
            directionPOV1 = 1;
        else if(instance->getPOV(joy1ID,0) == 270)
            directionPOV1 = -1;
        else
            directionPOV1 = 0;
    }
    else
        directionPOV1 = 0;

    if(v1 >= 0)
    {
        if(instance->getButton(joy1ID,bolinha))
        {
            v1 -= 0.5;
            if(v1 <= vMin1)
                v1 = vMin1;
        }
        if(instance->getButton(joy1ID,triangulo))
        {
            v1 += 0.5;
            if(v1 >= vMax1)
                v1 = vMax1;
        }
    }
    else
    {
        if(instance->getButton(joy1ID,bolinha))
        {
            v1 += 0.5;
            if(v1 >= vMin1)
                v1 = vMin1;
        }
        if(instance->getButton(joy1ID,triangulo))
        {
            v1 -= 0.5;
            if(v1 <= -vMax1)
                v1 = -vMax1;
        }
    }


    if(instance->getButton(joy1ID,xis) == 1 || instance->getButton(joy1ID,quadrado) == 1 )
    {
        if(instance->getButton(joy1ID,xis))
            v1 = fabs(v1);
        else
            v1 = -fabs(v1);

        startStatus1 = true;
    }
    else
    {
        startStatus1 = false;
    }

    //    float ct = 0.75;
    //    float teempo = (float) (clock() - clockStart)/CLOCKS_PER_SEC;
    //    if (teempo<=ct)
    //    {
    //        v1=teempo*v1/ct;
    //    }

    vTurn = 0.23*v1;
    if(direction1 > 0 || directionPOV1 > 0)
    {
        if(v1 > 0)
        {
            vLeft1 = v1 + vTurn;
            vRight1 = v1 - vTurn;
        }
        if(v1 < 0)
        {
            vLeft1 = v1 - vTurn;
            vRight1 = v1 + vTurn;
        }
    }
    else if (direction1 < 0 || directionPOV1 < 0 )
    {
        if(v1 > 0)
        {
            vLeft1 = v1 - vTurn;
            vRight1 = v1 + vTurn;
        }
        if(v1 < 0)
        {
            vLeft1 = v1 + vTurn;
            vRight1 = v1 - vTurn;
        }
    }
    else
    {
        vLeft1 = v1;
        vRight1 = v1;
    }
    if(instance->getButton(joy1ID,l_) == 1 || instance->getButton(joy1ID,r_) == 1 )
    {
        if(instance->getButton(joy1ID,l_) == 1)
        {
            vLeft1 = -vGiro;
            vRight1 = vGiro;
        }
        if(instance->getButton(joy1ID,r_) == 1)
        {
            vLeft1 = vGiro;
            vRight1 = -vGiro;
        }
        startStatus1 == true;
    }
}

void MainWindow::control2()
{
    if(instance->getNumAxes(joy2ID) > 0)
        direction2 = instance->getAxis(joy2ID,leftX);
    else
        direction2 = 0;

    if(instance->getNumPOVs(joy2ID) > 0)
    {
        if(instance->getPOV(joy2ID,0) == 90)
            directionPOV2 = 1;
        else if(instance->getPOV(joy2ID,0) == 270)
            directionPOV2 = -1;
        else
            directionPOV2 = 0;
    }
    else
        directionPOV2 = 0;

    if(v2 >= 0)
    {
        if(instance->getButton(joy2ID,bolinha))
        {
            v2 -= 0.5;
            if(v2 <= vMin2)
                v2 = vMin2;
        }
        if(instance->getButton(joy2ID,triangulo))
        {
            v2 += 0.5;
            if(v2 >= vMax2)
                v2 = vMax2;
        }
    }
    else
    {
        if(instance->getButton(joy2ID,bolinha))
        {
            v2 += 0.5;
            if(v2 >= vMin2)
                v2 = vMin2;
        }
        if(instance->getButton(joy2ID,triangulo))
        {
            v2 -= 0.5;
            if(v2 <= -vMax2)
                v2 = -vMax2;
        }
    }


    if(instance->getButton(joy2ID,xis) == 1 || instance->getButton(joy2ID,quadrado) == 1 )
    {
        if(instance->getButton(joy2ID,xis))
            v2 = fabs(v2);
        else
            v2 = -fabs(v2);

        startStatus2 = true;
    }
    else
    {
        startStatus2 = false;
    }

    vTurn = 0.23*v2;
    if(direction2 > 0 || directionPOV2 > 0)
    {
        if(v2 > 0)
        {
            vLeft2 = v2 + vTurn;
            vRight2 = v2 - vTurn;
        }
        if(v2 < 0)
        {
            vLeft2 = v2 - vTurn;
            vRight2 = v2 + vTurn;
        }
    }
    else if (direction2 < 0 || directionPOV2 < 0)
    {
        if(v2 > 0)
        {
            vLeft2 = v2 - vTurn;
            vRight2 = v2 + vTurn;
        }
        if(v2 < 0)
        {
            vLeft2 = v2 + vTurn;
            vRight2 = v2 - vTurn;
        }
    }
    else
    {
        vLeft2 = v2;
        vRight2 = v2;
    }
    if(instance->getButton(joy2ID,l_) == 1 || instance->getButton(joy2ID,r_) == 1 )
    {
        if(instance->getButton(joy2ID,l_) == 1)
        {
            vLeft2 = -vGiro;
            vRight2 = vGiro;
        }
        if(instance->getButton(joy2ID,r_) == 1)
        {
            vLeft2 = vGiro;
            vRight2 = -vGiro;
        }
        startStatus2 == true;
    }
}

void MainWindow::control3()
{
    if(instance->getNumAxes(joy3ID) > 0)
        direction3 = instance->getAxis(joy3ID,leftX);
    else
        direction3 = 0;

    if(instance->getNumPOVs(joy3ID) > 0)
    {
        if(instance->getPOV(joy3ID,0) == 90)
            directionPOV3 = 1;
        else if(instance->getPOV(joy3ID,0) == 270)
            directionPOV3 = -1;
        else
            directionPOV3 = 0;
    }
    else
        directionPOV3 = 0;

    if(v3 >= 0)
    {
        if(instance->getButton(joy3ID,bolinha))
        {
            v3 -= 0.5;
            if(v3 <= vMin3)
                v3 = vMin3;
        }
        if(instance->getButton(joy3ID,triangulo))
        {
            v3 += 0.5;
            if(v3 >= vMax3)
                v3 = vMax3;
        }
    }
    else
    {
        if(instance->getButton(joy3ID,bolinha))
        {
            v3 += 0.5;
            if(v3 >= vMin3)
                v3 = vMin3;
        }
        if(instance->getButton(joy3ID,triangulo))
        {
            v3 -= 0.5;
            if(v3 <= -vMax3)
                v3 = -vMax3;
        }
    }


    if(instance->getButton(joy3ID,xis) == 1 || instance->getButton(joy3ID,quadrado) == 1 )
    {
        if(instance->getButton(joy3ID,xis))
            v3 = fabs(v3);
        else
            v3 = -fabs(v3);

        startStatus3 = true;
    }
    else
    {
        startStatus3 = false;
    }

    vTurn = 0.23*v3;
    if(direction3 > 0 || directionPOV3 > 0)
    {
        if(v3 > 0)
        {
            vLeft3 = v3 + vTurn;
            vRight3 = v3 - vTurn;
        }
        if(v3 < 0)
        {
            vLeft3 = v3 - vTurn;
            vRight3 = v3 + vTurn;
        }
    }
    else if (direction3 < 0 || directionPOV3 < 0)
    {
        if(v3 > 0)
        {
            vLeft3 = v3 - vTurn;
            vRight3 = v3 + vTurn;
        }
        if(v3 < 0)
        {
            vLeft3 = v3 + vTurn;
            vRight3 = v3 - vTurn;
        }
    }
    else
    {
        vLeft3 = v3;
        vRight3 = v3;
    }
    if(instance->getButton(joy3ID,l_) == 1 || instance->getButton(joy3ID,r_) == 1 )
    {
        if(instance->getButton(joy3ID,l_) == 1)
        {
            vLeft3 = -vGiro;
            vRight3 = vGiro;
        }
        if(instance->getButton(joy3ID,r_) == 1)
        {
            vLeft3 = vGiro;
            vRight3 = -vGiro;
        }
        startStatus3 == true;
    }
}

float MainWindow::ajusteVR10FRENTE(float left)
{
    float aux = left;
    float result = 0;
    //return -6*pow(10,-8)*pow(aux,5) + 1*pow(10,-5)*pow(aux,4) - 0.001*pow(aux, 3) + 0.067*pow(aux,2) - 0.592*aux +14.80;
    //    return -0.0007*aux*aux + 0.8113*aux + 7.174;
    result = -2.5*pow(10,-3)*pow(aux,2) + 1.2812*aux - 3.4166;
    cout << "saida: " << result << endl;
    return result;
}

float MainWindow::ajusteVR10RE(float left)
{
    float aux = left;
    //return -6*pow(10,-8)*pow(aux,5) + 1*pow(10,-5)*pow(aux,4) - 0.001*pow(aux, 3) + 0.067*pow(aux,2) - 0.592*aux +14.80;
    //    return -0.0007*aux*aux + 0.8113*aux + 7.174;
    return -7*(10,-6)*pow(aux,4) - 1.7*pow(10,-3)*pow(aux,3) + 1.409*pow(10,-1)*pow(aux,2) - 3.711*aux - 61.792;
}

float MainWindow::ajusteMotor(float aux, int r)
{
    float result = 0;
    if(r == 10)
    {
        result = -2.5*pow(10,-3)*pow(aux,2) + 1.2812*aux - 3.4166;
    }
    cout << "saida: " << result << endl;
    return result;
}
