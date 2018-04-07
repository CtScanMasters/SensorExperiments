#include "SerialCTScanExperiments.h"
#include "SerialToArduino.h"
#include <qdebug.h>

SerialCTScanExperiments::SerialCTScanExperiments(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_sensorCounter = 1;
	m_portConnectState = false;

	connect(ui.serialWidget, SIGNAL(newDataSignal()), this, SLOT(getData()));
	connect(ui.pushButton1, SIGNAL(clicked()), this, SLOT(buttonAction()));
	connect(ui.pushButton2, SIGNAL(clicked()), this, SLOT(buttonAction()));
	connect(ui.pushButton3, SIGNAL(clicked()), this, SLOT(buttonAction()));
	connect(ui.pushButtonTimer, SIGNAL(clicked()), this, SLOT(toggleTimer()));
	connect(&m_sensorTimer, SIGNAL(timeout()), this, SLOT(readSensors()));
	connect(ui.serialWidget, SIGNAL(portConnected(bool)), SLOT(portConnectState(bool)));

}

void SerialCTScanExperiments::sendCommand(QString commandString)
{
	QByteArray bytesData;
	bytesData = commandString.toUtf8();
	
	ui.serialWidget->addToWriteBuffer(bytesData);
	ui.serialWidget->writeData();

	logMessage(QString("OUT>> %1").arg(commandString));
}

void SerialCTScanExperiments::getData()
{
	QByteArray dataInBuffer;
	ui.serialWidget->getFromReadBuffer(dataInBuffer);

	QString dataInString = dataInBuffer.toHex();	

	quint8 selector = dataInBuffer[2];
	dataInBuffer.remove(2, 1);
	bool ok = false;
	unsigned int data = dataInBuffer.toHex().toInt(&ok, 16);

	qDebug() << "Sel: " << selector << "data: " << data << "array: " <<dataInBuffer;
	logMessage(QString("IN<< sensorselect: %1 size: %2, data: %3").arg(selector).arg(dataInBuffer.size()).arg(dataInString));

	switch (selector)
	{
	case 1:
		ui.progressBar1->setValue(data);
		break;

	case 2:
		ui.progressBar2->setValue(data);
		break;

	case 3:
		ui.progressBar3->setValue(data);
		break;

	case 4:
		ui.progressBar4->setValue(data);
		break;
	}
}

void SerialCTScanExperiments::buttonAction()
{
	QPushButton *pushButton = (QPushButton *)sender();
	sendCommand(pushButton->text());
}


void SerialCTScanExperiments::readSensors()
{

	ui.progressBar1->setMaximum(ui.spinBox1->value());
	ui.progressBar2->setMaximum(ui.spinBox2->value());
	ui.progressBar3->setMaximum(ui.spinBox3->value());
	ui.progressBar4->setMaximum(ui.spinBox4->value());


	if (!m_portConnectState)
	{
		m_sensorTimer.stop();
		return;
	}


	switch (m_sensorCounter)
	{
	case 1: sendCommand("<SENS-1>");
		break;

	case 2: sendCommand("<SENS-2>");
		break;

	case 3: sendCommand("<SENS-3>");
		break;

	case 4: sendCommand("<SENS-4>");
		break;

	}

	m_sensorCounter++;
	if (m_sensorCounter > 4)
	{
		m_sensorCounter = 1;
	}

}
void SerialCTScanExperiments::toggleTimer()
{
	if (m_sensorTimer.isActive())
	{
		m_sensorTimer.stop();
		return;
	}
	else if(m_portConnectState)
	{
		m_sensorTimer.start(ui.spinBoxTimer->value());
	}

	
}

void SerialCTScanExperiments::portConnectState(bool state)
{
	m_portConnectState = state;

}

void SerialCTScanExperiments::logMessage(QString message)
{
	ui.plainTextEdit->appendPlainText(message);
	qDebug() << message;
}