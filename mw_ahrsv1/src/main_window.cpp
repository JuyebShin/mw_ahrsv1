/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/mw_ahrsv1/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mw_ahrsv1 {

using namespace Qt;
using namespace std;

QByteArray Rx_data;
QByteArray Tx_data;
unsigned char TxBuffer[5] = {0, };


/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    qnode.init();
    if(!open_serial())
        exit(0);

    init_mw_ahrsv1();

    QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(mw_ahrsv1CallBack()));
    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
}

bool MainWindow::open_serial()
{
    serial = new QSerialPort(this);
    serial -> setPortName("USBIMU");
    serial -> setBaudRate(QSerialPort::Baud115200);
    serial -> setDataBits(QSerialPort::Data8);
    serial -> setParity(QSerialPort::NoParity);
    serial -> setStopBits(QSerialPort::OneStop);
    serial -> setFlowControl(QSerialPort::NoFlowControl);
    if(!serial -> open(QIODevice::ReadWrite))
    {
        cout<<"**********Serial port open fail!!**********"<<endl;
        return false;
    }
    else
        return true;
}


void MainWindow::init_mw_ahrsv1()
{
    TxBuffer[0] = A;     // a
    TxBuffer[1] = N;     // n
    TxBuffer[2] = G;     // g
    TxBuffer[3] = CR;    // CR
    TxBuffer[4] = LF;    // LF

    for(int i = 0; i < 5; i++)
    {
        Tx_data.push_back(TxBuffer[i]);
    }
    serial -> write(Tx_data,5);
}

void MainWindow::mw_ahrsv1CallBack()
{
    serial -> write(Tx_data,5);
    Rx_data = serial->readAll();
    char *Rx_Buf = Rx_data.data();
    char *ptr = strtok(Rx_Buf," \n");
    cout<<ptr<<endl;
//    for(int i = 0; i < 3; i++)
//    {
//        ptr = strtok(NULL, " \n");
//        if(i == 0)
//            euler.roll = atof(ptr);
//        else if(i == 1)
//            euler.pitch = atof(ptr);
//        else if(i == 2)
//            euler.yaw = atof(ptr);
//    }

    int i = 0;
    while(ptr != NULL)
    {
        if(i == 1)
            euler.roll = atof(ptr);
        else if(i == 2)
            euler.pitch = atof(ptr);
        else if(i == 3)
            euler.yaw = atof(ptr);
        ptr = strtok(NULL, " \n");
        i++;
    }
    imu_msg.roll = euler.roll;
    imu_msg.pitch = euler.pitch;
    imu_msg.yaw = euler.yaw;
    std::cout << "roll = " << euler.roll << std::endl;
    std::cout << "pitch = " << euler.pitch << std::endl;
    std::cout << "yaw = " << euler.yaw << std::endl;
    std::cout << std::endl;
    qnode.Imu_pub.publish(imu_msg);
    serial -> write(Tx_data,5);
    Rx_data.clear();
}

MainWindow::~MainWindow() {
    serial->close();
}



}  // namespace mw_ahrsv1

