// Author: Olli-Pekka Heinisuo
// License: MIT

// Arduino temperature station w/ ethernet shield
// Reads temperature (Celsius) values from DS18B20 sensors
// and sends them to remote server

#include <SPI.h>
#include <OneWire.h>
#include <Ethernet.h>
#include <EthernetClient.h>

#define sensors 3  // amount of sensors

// Ethernet shield mac-address
byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Server where to connect
byte server[] = { xxx, xxx, xxx, xxx };

EthernetClient client;

void setup()
{
	Serial.begin(9600);

	// Arduino ethernet shield should work with DHCP, if only the mac-address is given

	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP.");
		Serial.println();
	}
	else {
		Serial.println("DHCP configuration successful.");
		Serial.println();
	}
}

OneWire ds(9);  // initialize OneWire library to pin 9

int counter = 0; // for the amount of sensors

void loop(void) {

	int MS, LS, reading, SignBit;
	float temp;
	String sign = "";

	if (counter == 0) {
		
		Serial.println();
		Serial.println("Connecting...");

		if (client.connect(server, 80)) {  // connect to the specified server
			Serial.println("Connected.");
		}
		else {
			Serial.println("Connection failed.");
		}
		
	}
	else if (counter == sensors) {
		counter = 0;
	}

	byte i;
	byte data[12];	// sensor data
	byte addr[8];  	// sensor address

	if ( !ds.search(addr)) {    // if no more addresses, reset
		ds.reset_search();
		return;
	}

	if ( OneWire::crc8( addr, 7) != addr[7]) {  // check CRC
		Serial.print("CRC is not valid.\n");
		return;
	}

	ds.reset();
	ds.select(addr);
	ds.write(0x44,1);		// start conversion, with parasite power on at the end

	delay(750);				// let's give some time for the sensor to do conversion

	ds.reset();				// reset
	ds.select(addr);		// select address
	ds.write(0xBE);			// Read scratchpad

	for ( i = 0; i < 9; i++) {
		data[i] = ds.read();
	}

	LS = data[0];                        // least significant byte
	MS = data[1];                        // most significant byte
	reading = (MS << 8) + LS;            // shift MS byte to the left and add LS byte
	SignBit = reading & 0x8000;          // test most significant bit by performing and-operation

	if (SignBit)  {                       // negative

		reading = (reading ^ 0xffff) + 1; 
		// two's complement 
		// (bitwise exclusive or + 1
		// or in other terms, compute Treading's bitwise negation and add 1
	}

	// debugging Serial.print(reading);

	temp = reading * 0.0625; // multiply by the 12-bit precision
	String sensor = "";

	// select sensor, these have to be set up manually for now..
	if ( counter == 0) {
		Serial.print("ulko:");
		sensor = "ulko";
	}
	else if ( counter == 1) {
		Serial.print("kone:");
		sensor = "kone";
	}
	else if ( counter == 2) {
		Serial.print("sisa:");
		sensor = "sisa";
	}

	++counter;

	String parameter = "&sensor="+sensor;

	client.print("GET temps.php?temp="); // your server address and folder...
	client.print(temp);
	client.print(parameter);
	client.println(" HTTP/1.1");
	client.println("Host: HOST");  // your server hostname
	client.println();
	
	Serial.println("Data was sent to the remote server.");

	if ( counter == sensors ) { // 5 minute delay between readings
		client.stop();
		Serial.println("Connection closed.");
		Serial.println();
		Serial.println("Sleeping for 5 mins.");
		delay(300000);
	}
}
