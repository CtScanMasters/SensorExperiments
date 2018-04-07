#ifndef SERIALTOARDUINO_H
#define SERIALTOARDUINO_H

#include <QWidget>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <qlist.h>
#include <qgridlayout.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qplaintextedit.h>



class SerialToArduino : public QWidget
{
	Q_OBJECT

public:
	SerialToArduino(QWidget *parent = 0);
	~SerialToArduino();

	int getSelectedBaudRate();
	QString getSelectedComPort();
	void setPortSettings();	
	bool portIsConnected();
	void addToWriteBuffer(QByteArray byteArray);
	void getFromReadBuffer(QByteArray& byteArray);

private:
	QComboBox *comboBoxPortSelect;
	QComboBox *comboBoxBaudSelect;
	QPushButton *pushButtonConnect;
	QPushButton *pushButtonDisconnect;
	QLabel *statusLabel;
	
	bool connectedFlag;
	bool isReadingFlag;
		
	QSerialPort *serial;
	QStringList *baudRateSelections;

	QMessageBox *infoBox;
	QGridLayout *gridLayoutMain;

	QList<QByteArray> dataInBufferList;
	QList<QByteArray> dataOutBufferList;

private slots:
	bool connectSerialPort();
	void disconnectSerialPort();
	void readData();

public slots:
	void writeData();

signals:
	void newDataSignal();
	void portConnected(bool state);



};

#endif // SERIALTOARDUINO_H
