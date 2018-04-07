#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SerialCTScanExperiments.h"
#include <qtimer.h>


class SerialCTScanExperiments : public QMainWindow
{
	Q_OBJECT

public:
	SerialCTScanExperiments(QWidget *parent = Q_NULLPTR);
	void sendCommand(QString commandString);
	QTimer m_sensorTimer;

private:
	Ui::SerialCTScanExperimentsClass ui;
	quint16 m_sensorCounter;
	bool m_portConnectState;
	void logMessage(QString message);


private slots:	
	void getData();
	void buttonAction();
	void readSensors();
	void toggleTimer();
	void portConnectState(bool state);



};
