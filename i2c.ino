#include <Wire.h> // I2C Library for Communication
#include <Servo.h> // Servo Library for controlling Servo Motors

#define ADDRESS 0x08 // 0x08 is the I2C address of the slave in this case arduino uno

byte array1[4] = {127,127,127,127}; // init array to receive data from pi
int panPos = 0; // init variable for pan servo's angle
int tiltPos = 0; // init variable for tilt servo's angle

// Define constants for hardware pins on the arduino to connect to I/O
#define PANI 7      // pin #7 connected to Pan Servo
#define TILTI 6     // pin #6 connected to Tilt Servo
#define ABCK  8     // pin #8 connected to Motor 1 Reverse - Motor Driver
#define AFWD  9     // pin #9 connected to Motor 1 Forward - Motor Driver
#define BBCK  10    // pin #10 connected to Motor 2 Reverse - Motor Driver
#define BFWD  11    // pin #11 connected to Motor 2 Forward - Motor Driver
#define LED 13      // pin #13 connected to the onboard LED of Arduino UNO

//Initialize Servo objects for pan & tilt servo
Servo pan;    //  init pan servo object
Servo tilt;   //  init tilt servo object 


// Setup function begins. Runs once.
void setup(void) {
  Wire.begin(ADDRESS);  // Init I2C communication
  Wire.onReceive(msg_received); // Setup Interrupt to handle I2C data
  Serial.begin(112500); // Init Serial communication at 112500 baud rate

// Set pins to as Outputs
  pinMode(LED, OUTPUT);   // LED pin set to Output
  pinMode(ABCK, OUTPUT);  // ABCK pin set to Output
  pinMode(AFWD, OUTPUT);  // AFWD pin set to Output
  pinMode(BBCK, OUTPUT);  // BBCK pin set to Output
  pinMode(BFWD, OUTPUT);  // BFWD pin set to Output
  
// attach the servo object to the servo pins
  pan.attach(PANI);
  tilt.attach(TILTI);
}


// loop function begins, Loops forever.
void loop() {
  
  for(int i=3;i>=0;i--) // loop 3 - 0
  {
  Serial.println(array1[i]); // prints array1 to serial terminal
  }
  
  pan.write(map(array1[1],0x00,0XFF,0,180)); // maps the value received over I2C to 0 - 180 and writes the angle to the servo.
  tilt.write(map(array1[0],0xFF,0x00,0,180));
  delay(20); // produces a time delay of 20 ms
  
  // nod yes with the servo mechanism.
  if(array1[1] == 0xEF) {   // check if command to nod yes is received
    pan.write(120);
    delay(200);
    pan.write(60);
    delay(200);
    pan.write(120);
    delay(200);
    pan.write(60);
    delay(200);
    pan.write(90);
  }
  
  // nod no with the servo mechanism.
  else if(array1[0] == 0x01) {    // check if command to nod yes is received
    tilt.write(120);
    delay(200);
    tilt.write(60);
    delay(200);
    tilt.write(120);
    delay(200);
    tilt.write(60);
    delay(200);
    tilt.write(90);
  }
  
  // check to move forward with the Motor Driver
  if(array1[3] == 0xFF) {
    Serial.println("Forward");
  digitalWrite(AFWD, HIGH);
  digitalWrite(ABCK, LOW);
  delay(20);
  }
  // check to move backward with the Motor Driver
  else if(array1[3] == 0x00) {
    Serial.println("back");
  digitalWrite(AFWD, LOW);
  digitalWrite(ABCK, HIGH);
  delay(20);
  }
  // Stop if all checks fail
  else {
    Serial.println("stop");
    digitalWrite(AFWD, LOW);
    digitalWrite(ABCK, LOW);
    delay(20);
  }
  // Check to Turn Right
  if(array1[2] == 0xFF) {
    Serial.println("Right");
  digitalWrite(BFWD, HIGH);
  digitalWrite(BBCK, LOW);
  delay(20);
  }
  // Check to Turn Left
  else if(array1[2] == 0x00) {
  Serial.println("Left");
  digitalWrite(BFWD, LOW);
  digitalWrite(BBCK, HIGH);
  delay(20);
  }
  // go straight if all checks fail
  else {
    Serial.println("stra");
    digitalWrite(BFWD, LOW);
    digitalWrite(BBCK, LOW);
    delay(20);
  }
}
// Loop function ends


// function to handle I2C data
void msg_received(int numBytes)
{
  byte byteCount = 0;
  byteCount = Wire.available(); // grab the byte count when data is available
  Serial.print("------ Slave address > ");
  Serial.println(ADDRESS); // print data to the serial monitor
  Serial.print("Byte Count ");
  Serial.println(byteCount);
  digitalWrite(LED, HIGH); // set onboard LED high while receiving data
  while(byteCount > 0) // loop through all the bytes received
  {
    byteCount -= 1;
    if (byteCount == 4) { // check to print the command byte to the serial monitor
      byte x = Wire.read();
      Serial.println("Command Byte ");
    }
    else {
      array1[byteCount] = Wire.read(); // assigns the byte value to array1
      Serial.println(array1[byteCount]); // prints updated values to the serial monitor
    }
  } 
}
