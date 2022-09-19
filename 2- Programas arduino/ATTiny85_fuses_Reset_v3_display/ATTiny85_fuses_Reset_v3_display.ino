#include "Arduino.h"
// AVR High-voltage Serial Programmer
// Originally created by Paul Willoughby 03/20/2010
// www.rickety.us slash 2010/03/arduino-avr-high-voltage-serial-programmer/
// Inspired by Jeff Keyzer mightyohm.com
// Serial Programming routines from ATtiny25/45/85 datasheet

// Desired fuse configuration
#define  HFUSE  0xDF   // Defaults for ATtiny25/45/85
#define  LFUSE  0x62

#define  RST     A5    // Output to level shifter for !RESET from transistor to Pin 1
#define  CLKOUT  6    // Connect to Serial Clock Input (SCI) Pin 2
#define  DATAIN  3    // Connect to Serial Data Output (SDO) Pin 7
#define  INSTOUT 4    // Connect to Serial Instruction Input (SII) Pin 6
#define  DATAOUT  5    // Connect to Serial Data Input (SDI) Pin 5
#define  VCC      2    // Connect to VCC Pin 8

// Added by Ralph S Bacon just for enhanced user experience
#define  rst_off	  7		// led resert off/pin 1 gpio
#define  PWRLED	  9		// Power on LED
#define	 rst_on	  8		// led reset on/default

int inData = 0;         // incoming serial byte AVR
int targetValue = HFUSE;

//    display
#include <TM1637Display.h>

#define CLK 14
#define DIO 15

TM1637Display Display(CLK, DIO);

const uint8_t fuse[] = {
  SEG_A | SEG_E | SEG_F | SEG_G,
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G
};

const uint8_t rst[] = {
  SEG_E | SEG_G,
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,
  SEG_F | SEG_D | SEG_E | SEG_G
};

const uint8_t gpio[] = {
  SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,
  SEG_C | SEG_B,
  SEG_C | SEG_D | SEG_E | SEG_A | SEG_B | SEG_F
};
//

// botones
byte push = false;
int button1 = 0;
int button2 = 0;
//

void setup() {
	// Set up control lines for HV parallel programming
	pinMode(VCC, OUTPUT);
	pinMode(RST, OUTPUT);
	pinMode(DATAOUT, OUTPUT);
	pinMode(INSTOUT, OUTPUT);
	pinMode(CLKOUT, OUTPUT);
	pinMode(DATAIN, OUTPUT);  // configured as input when in programming mode

	// LEDS
	pinMode(PWRLED, OUTPUT);
	pinMode(rst_on, OUTPUT);
  pinMode(rst_off, OUTPUT);
	digitalWrite(PWRLED, HIGH);
	digitalWrite(rst_on, LOW);
	digitalWrite(rst_off, LOW);

 //display
// pinMode(CLK, OUTPUT);
// pinMode(DIO, OUTPUT);
Display.setBrightness(5);
 Display.clear();
 //

	// Initialize output pins as needed
	digitalWrite(RST, HIGH);  // Level shifter is inverting, this shuts off 12V

	// start serial port at 9600 bps:
	Serial.begin(9600);
}

void loop() {

	switch (establishContact()) {
		case 49:
			targetValue = HFUSE;
			break;
		case 50:
			targetValue = 0x5F;
			break;
		default:
			targetValue = HFUSE;
	}

	Serial.println("Entrando al modo de programación\n");

	// Initialize pins to enter programming mode
	pinMode(DATAIN, OUTPUT);  //Temporary
	digitalWrite(DATAOUT, LOW);
	digitalWrite(INSTOUT, LOW);
	digitalWrite(DATAIN, LOW);
	digitalWrite(RST, HIGH);  // Level shifter is inverting, this shuts off 12V

	// Enter High-voltage Serial programming mode
	digitalWrite(VCC, HIGH);  // Apply VCC to start programming process
	delayMicroseconds(20);
	digitalWrite(RST, LOW);   //Turn on 12v
	delayMicroseconds(10);
	pinMode(DATAIN, INPUT);   //Release DATAIN
	delayMicroseconds(300);

	//Programming mode
	int hFuse = readFuses();

	//Write hfuse if not already the value we want 0xDF (to allow RST on pin 1)
	if (hFuse != targetValue) {
		delay(1000);
		Serial.print("configurando hfuse como ");Serial.println(targetValue, HEX);
		shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x40, 0x4C);

		// The default RESET functionality
		//shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, HFUSE, 0x2C);

		// this turns the RST pin 1 into a (weak) IO port
		//shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x5F, 0x2C);

		// User selected option
		shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, targetValue, 0x2C);

		shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x74);
		shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x7C);
	}

	//Write lfuse
	delay(1000);
	Serial.println("configurando lfuse\n");
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x40, 0x4C);
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, LFUSE, 0x2C);
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x64);
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x6C);

	// Confirm new state of play
	hFuse = readFuses();

	digitalWrite(CLKOUT, LOW);
	digitalWrite(VCC, LOW);
	digitalWrite(RST, HIGH);   //Turn off 12v

	// Let user know we're done
 if (hFuse == HFUSE){
	digitalWrite(rst_on, HIGH);
 Display.setSegments(rst, 4, 0);
 }
 else if(hFuse == 0x5F){
  digitalWrite(rst_off, HIGH);
  Display.setSegments(gpio, 4, 0);
 }
  digitalWrite(PWRLED, LOW);
  delay(500);
  digitalWrite(PWRLED, HIGH);
  delay(500);
  digitalWrite(PWRLED, LOW);
  delay(500);
  digitalWrite(PWRLED, HIGH);
  delay(500);
  digitalWrite(PWRLED, LOW);
  delay(500);
  digitalWrite(PWRLED, HIGH);
  delay(500);

	Serial.println("\nProgramación completada. Presiona RESET para voler a iniciar el programador.");
	while (1==1){};
}

int establishContact() {
	Serial.println("Enciende la fuente de 12v/\n\nPuedes ACTIVAR el pin de reset (como RST) "
      "para permitir la reprogramación del chip\no DESACTIVARLO para configurarlo como una salida (débil)de proposito general (GPIO).\n");

  // We must get a 1 or 2 to proceed
  int reply;

  do {
    Serial.println("Presiona 1 para ACTIVAR el pin RST (de vuelta a la normalidad)");
    Serial.println("Presiona 2 para DESACTIVAR el pin RST (configurarlo como pin GPIO)");
    Display.setSegments(fuse, 4, 0);
    while (!Serial.available()) {
      // Wait for user input
      button1 = digitalRead(16);
      button2 = digitalRead(17);
      if(button1 == HIGH){
        reply = 49;
        push = true;
        break;
      }
      if(button2 == HIGH){
        reply = 50;
        push = true;
        break;
      }
      delay(50);
    }
    if (push == false){
    reply = Serial.read();
    }
  }
  while (!(reply == 49 || reply == 50));
  return reply;
}

int shiftOut2(uint8_t dataPin, uint8_t dataPin1, uint8_t clockPin, uint8_t bitOrder, byte val, byte val1) {
	int i;
	int inBits = 0;
	//Wait until DATAIN goes high
	while (!digitalRead(DATAIN)) {
		// Nothing to do here
	}

	//Start bit
	digitalWrite(DATAOUT, LOW);
	digitalWrite(INSTOUT, LOW);
	digitalWrite(clockPin, HIGH);
	digitalWrite(clockPin, LOW);

	for (i = 0; i < 8; i++) {

		if (bitOrder == LSBFIRST) {
			digitalWrite(dataPin, !!(val & (1 << i)));
			digitalWrite(dataPin1, !!(val1 & (1 << i)));
		}
		else {
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));
			digitalWrite(dataPin1, !!(val1 & (1 << (7 - i))));
		}
		inBits <<= 1;
		inBits |= digitalRead(DATAIN);
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);

	}

	//End bits
	digitalWrite(DATAOUT, LOW);
	digitalWrite(INSTOUT, LOW);
	digitalWrite(clockPin, HIGH);
	digitalWrite(clockPin, LOW);
	digitalWrite(clockPin, HIGH);
	digitalWrite(clockPin, LOW);

	return inBits;
}

// Returns the value of the HFUSE
int readFuses() {
	Serial.println("Leyendo fusibles");

	//Read lfuse
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x04, 0x4C);
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x68);
	inData = shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x6C);
	Serial.print("lfuse = ");
	Serial.println(inData, HEX);

	//Read hfuse
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x04, 0x4C);
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x7A);
	inData = shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x7E);
	Serial.print("hfuse = ");
	Serial.println(inData, HEX);
	int hFuse = inData;

	//Read efuse
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x04, 0x4C);
	shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x6A);
	inData = shiftOut2(DATAOUT, INSTOUT, CLKOUT, MSBFIRST, 0x00, 0x6E);
	Serial.print("efuse = ");
	Serial.println(inData, HEX);
	Serial.println();

	return hFuse;
}
