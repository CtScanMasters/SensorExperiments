#include "SerialCTScanExperiments.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SerialCTScanExperiments w;
	w.show();
	return a.exec();
}
