
const int inputBufferSize = 8;
const int outputBufferSize = 3;

bool newData = true;
uint8_t handshakeData = 0;

char inPutBuffer[inputBufferSize];

#define LED1	A2
#define LED2	3
#define LED3	13
#define SENSE1	A0
#define SENSE2	A1
#define SENSE3	A4

#define STATUSLED 13


bool LED1_status = false;
bool LED2_status = false;
bool LED3_status = false;
bool STATUSLED_status = false;




uint8_t outPutBuffer[outputBufferSize] =
{
	0,0,0
};


void setup()
{
	Serial1.begin(500000);

	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(STATUSLED, OUTPUT);

	pinMode(SENSE1, INPUT);
	pinMode(SENSE2, INPUT);
	pinMode(SENSE3, INPUT);
	
	digitalWrite(LED1, LED1_status);
	digitalWrite(LED2, LED2_status);
	digitalWrite(LED3, LED3_status);
	digitalWrite(STATUSLED, STATUSLED_status);

	//Clear Serial1 buffer
	while (Serial1.available() > 0) {
		char t = Serial1.read();
	}

}

void loop()
{

	Serial1.readBytes(inPutBuffer, inputBufferSize);
	String commandString = (char*)inPutBuffer;

	
	if (commandString)
	{

		if (commandString == "<LED1-->")
		{
			LED1_status = !LED1_status;
			digitalWrite(LED1, LED1_status);
			digitalWrite(STATUSLED, LED1_status);

		}
		if (commandString == "<LED2-->")
		{
			LED2_status = !LED2_status;
			digitalWrite(LED2, LED2_status);

		}
		if (commandString == "<LED3-->")
		{
			LED3_status = !LED3_status;
			digitalWrite(LED3, LED3_status);


		}
		if (commandString == "<SENS-1>")
		{
			uint16_t sensor = analogRead(SENSE1);
			outPutBuffer[1] = sensor & 0xFF;
			outPutBuffer[0] = sensor >> 8;
			outPutBuffer[2] = 1;

			Serial1.write(outPutBuffer, outputBufferSize);
			Serial1.flush();
		}
		if (commandString == "<SENS-2>")
		{
			uint16_t sensor = analogRead(SENSE2);
			outPutBuffer[1] = sensor & 0xFF;
			outPutBuffer[0] = sensor >> 8;
			outPutBuffer[2] = 2;

			Serial1.write(outPutBuffer, outputBufferSize);
			Serial1.flush();
		}
		if (commandString == "<SENS-3>")
		{
			uint16_t sensor = analogRead(SENSE3);
			outPutBuffer[1] = sensor & 0xFF;
			outPutBuffer[0] = sensor >> 8;
			outPutBuffer[2] = 3;

			Serial1.write(outPutBuffer, outputBufferSize);
			Serial1.flush();
		}
		if (commandString == "<SENS-4>")
		{
			uint16_t sensor = 0;
			outPutBuffer[1] = sensor & 0xFF;
			outPutBuffer[0] = sensor >> 8;
			outPutBuffer[2] = 4;

			Serial1.write(outPutBuffer, outputBufferSize);
			Serial1.flush();
		}
	}



	memset(inPutBuffer, 0, inputBufferSize);



}

