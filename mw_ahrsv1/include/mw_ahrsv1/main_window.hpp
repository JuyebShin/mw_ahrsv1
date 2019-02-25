/**
 * @file /include/mw_ahrsv1/main_window.hpp
 *
 * @brief Qt based gui for mw_ahrsv1.
 *
 * @date November 2010
 **/
#ifndef mw_ahrsv1_MAIN_WINDOW_H
#define mw_ahrsv1_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace mw_ahrsv1 {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();
        QSerialPort *serial;
        mw_ahrsv1::imu_msg imu_msg;
        bool open_serial();

public Q_SLOTS:
        void mw_ahrsv1CallBack();


private:
	Ui::MainWindowDesign ui;
	QNode qnode;
        void init_mw_ahrsv1();

        // ASCII CODE
        const unsigned char A = 0x61;
        const unsigned char N = 0x6E;
        const unsigned char G = 0x67;
        const unsigned char CR = 0x0D;
        const unsigned char LF = 0x0A;

        typedef struct
        {
            // Euler
            float roll;
            float pitch;
            float yaw;

        }Euler;
        Euler euler;
};

}  // namespace mw_ahrsv1

#endif // mw_ahrsv1_MAIN_WINDOW_H
