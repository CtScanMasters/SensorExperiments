#include "SerialToArduino.h"
#include <qdebug.h>

SerialToArduino::SerialToArduino(QWidget *parent)
	: QWidget(parent)
{

	//Configuration widget
	comboBoxPortSelect = new QComboBox(this);
	comboBoxBaudSelect = new QComboBox(this);
	pushButtonConnect = new QPushButton(this);
	pushButtonDisconnect = new QPushButton(this);
	statusLabel = new QLabel(this);
	baudRateSelections = new QStringList;
	gridLayoutMain = new QGridLayout(this);
	serial = new QSerialPort(this);
	infoBox = new QMessageBox(this);


	pushButtonConnect->setText("Connect");
	pushButtonDisconnect->setText("Disconnect");
	statusLabel->setText("Disconnected");

	comboBoxPortSelect->setFixedWidth(75);
	comboBoxBaudSelect->setFixedWidth(75);
	
	connectedFlag = false;
	infoBox->setWindowTitle("Message");

	connect(pushButtonConnect, SIGNAL(clicked()), this, SLOT(connectSerialPort()));
	connect(pushButtonDisconnect, SIGNAL(clicked()), this, SLOT(disconnectSerialPort()));
	connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
	
	*baudRateSelections << "500000" << "250000" << "200000" << "153600" << "128000" << "115200" << "57600" << "38400" << "19200" << "9600";
	comboBoxBaudSelect->addItems(*baudRateSelections);
	comboBoxBaudSelect->setCurrentText("500000");
	
	foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
	{
		comboBoxPortSelect->addItem(serialPortInfo.portName());
	}

	gridLayoutMain->addWidget(comboBoxPortSelect, 0, 0);
	gridLayoutMain->addWidget(comboBoxBaudSelect, 1, 0);
	gridLayoutMain->addWidget(pushButtonConnect, 0, 2);
	gridLayoutMain->addWidget(pushButtonDisconnect, 1, 2);
	gridLayoutMain->addWidget(statusLabel, 3, 0,1,3);

	disconnectSerialPort();
	
}

SerialToArduino::~SerialToArduino()
{
	disconnectSerialPort();
}




bool SerialToArduino::connectSerialPort() {

	QPushButton *newLabel = new QPushButton[10];

	setPortSettings();

	if (serial->isOpen() && serial->isReadable()) {

		serial->clear();
		connectedFlag = true;
		statusLabel->setText(QString("Connected to port: %1").arg(comboBoxPortSelect->currentText()));
		comboBoxPortSelect->setDisabled(true);
		comboBoxBaudSelect->setDisabled(true);
		pushButtonConnect->setDisabled(true);
		pushButtonDisconnect->setDisabled(false);
		
	}
	else if (comboBoxPortSelect->currentText() == 0) {

		infoBox->warning(this, "Serial device","No port selected", 1, 0, 0);
		connectedFlag = false;
	}
	else {
		infoBox->warning(this, "Serial device", QString("Unable to connect %1").
			arg(comboBoxPortSelect->currentText()), 1, 0, 0);
		connectedFlag = false;
	}

	emit portConnected(connectedFlag);
	return(connectedFlag);

}

void SerialToArduino::disconnectSerialPort() {

	connectedFlag = false;
	emit portConnected(connectedFlag);
	serial->close();	
	statusLabel->setText("Disconnected");
	comboBoxPortSelect->setDisabled(false);
	comboBoxBaudSelect->setDisabled(false);
	pushButtonConnect->setDisabled(false);
	pushButtonDisconnect->setDisabled(true);

}


int SerialToArduino::getSelectedBaudRate() {

	QString selectedBaud = comboBoxBaudSelect->currentText();
	return(selectedBaud.toInt());
}

QString SerialToArduino::getSelectedComPort() {

	return(comboBoxPortSelect->currentText());
}

void SerialToArduino::setPortSettings() {

	serial->setPortName(getSelectedComPort());
	serial->setBaudRate(getSelectedBaudRate());

	serial->open(QIODevice::ReadWrite);
	serial->setDataBits(QSerialPort::Data8);
	serial->setParity(QSerialPort::NoParity);
	serial->setStopBits(QSerialPort::OneStop);
	serial->setFlowControl(QSerialPort::NoFlowControl);

}

void SerialToArduino::readData()
{

	if (serial->isOpen() && serial->isReadable())
	{
		QByteArray dataIn;
		dataIn.append(serial->readAll());

		if (dataIn.size() < 2)
		{
			return;
		}
		else
		{
			dataInBufferList.prepend(dataIn);
			dataIn.clear();
			emit newDataSignal();
		}
	}
	else
	{
		infoBox->warning(this, "Serial device", QString("Unable to read %1").
			arg(comboBoxPortSelect->currentText()), 1, 0, 0);
	}


}

void SerialToArduino::writeData()
{
	if (serial->isOpen() && serial->isWritable())
	{
		if (dataOutBufferList.size() > 0)
		{
			serial->write(dataOutBufferList.last());
			serial->flush();

			dataOutBufferList.removeLast();
		}
		else
		{
			infoBox->warning(this, "Serial device", "Write buffer empty");
		}
	}
	else
	{
		infoBox->warning(this, "Serial device", QString("Unable to write %1").
			arg(comboBoxPortSelect->currentText()), 1, 0, 0);
	}

}

bool SerialToArduino::portIsConnected()
{
	return(connectedFlag);
}

void SerialToArduino::addToWriteBuffer(QByteArray byteArray)
{
	dataOutBufferList.prepend(byteArray);
}

void SerialToArduino::getFromReadBuffer(QByteArray& byteArray)
{
	if (dataInBufferList.size() > 0)
	{
		byteArray = dataInBufferList.last();		
		dataInBufferList.removeLast();
	}
	else
	{
		byteArray = 0;
	}

}

