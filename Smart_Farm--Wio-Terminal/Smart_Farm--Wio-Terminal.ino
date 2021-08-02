
//Libraries
#include "TFT_eSPI.h" //TFT LCD library 
#include "DHT.h" // DHT library 
#include <ServoAlternative.h>

//Definitions
#define DHTPIN A0 //Define signal pin of DHT sensor 
#define DHTTYPE DHT22 //Define DHT sensor type 
Servo myservo; // Define Servo

//Initializations
DHT dht(DHTPIN, DHTTYPE); //Initializing DHT sensor
TFT_eSPI tft; //Initializing TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); //Initializing buffer

bool ServoSenal = false;
int TrueServo = 180;
int FalseServo = 0;
int StatusServo = 0;
int ServoAction = 0;

void setup() {
  Serial.begin(9600); //Start serial communication

  pinMode(WIO_LIGHT, INPUT); //Set light sensor pin as INPUT
  pinMode(WIO_BUZZER, OUTPUT); //Set buzzer pin as OUTPUT

  myservo.attach(D2); //Connect servo to Grove Digital Port
  myservo.write(180);

  tft.begin(); //Start TFT LCD
  tft.setRotation(3); //Set TFT LCD rotation

  tft.setFreeFont(&FreeSansBold24pt7b);
  tft.fillScreen(TFT_RED); //Red background
  tft.setTextColor(TFT_WHITE);          //sets the text colour to black
  tft.setTextSize(1);
  tft.drawString("MoonMakers", 13, 100);

  delay(3000);

  dht.begin(); //Start DHT sensor
  spr.createSprite(TFT_HEIGHT, TFT_WIDTH); //Create buffer
}

void loop() {
  int t = dht.readTemperature(); //Assign variable to store temperature
  int h = dht.readHumidity(); //Assign variable to store humidity
  int light = analogRead(WIO_LIGHT); //Assign variable to store light sensor values

  //Setting the title header
  spr.fillSprite(TFT_WHITE); //Fill background with white color
  spr.fillRect(0, 0, 320, 50, TFT_DARKGREEN); //Rectangle fill with dark green
  spr.setTextColor(TFT_WHITE); //Setting text color
  spr.setTextSize(3); //Setting text size
  spr.drawString("Smart Farm", 80, 15); //Drawing a text string

  // Makey Face
  spr.fillCircle(233, 135, 65, TFT_RED );
  spr.fillCircle(203,  110,  15, TFT_WHITE);
  spr.fillCircle(263, 110,  15, TFT_WHITE);

  // background rectangle
  spr.fillRect(150, 140, 175, 100, TFT_WHITE); // Lower right corner
  spr.drawFastVLine(150, 50, 190, TFT_DARKGREEN); //Drawing verticle line
  spr.drawFastHLine(0, 140, 320, TFT_DARKGREEN); //Drawing horizontal line

  //Setting temperature
  spr.setTextColor(TFT_BLACK);
  spr.setTextSize(2);
  spr.drawString("Temperature", 10, 65);
  spr.setTextSize(3);
  spr.drawNumber(t, 50, 95); //Display temperature values
  spr.drawString("C", 90, 95);

  //Setting humidity
  spr.setTextSize(2);
  spr.drawString("Humidity", 25, 160);
  spr.setTextSize(3);
  spr.drawNumber(h, 30, 190); //Display humidity values
  spr.drawString("%RH", 70, 190);

  //Setting light
  spr.setTextSize(2);
  spr.drawString("Light", 200, 160);
  spr.setTextSize(3);
  light = map(light, 0, 1023, 0, 100); //Map sensor values
  spr.drawNumber(light, 205, 190); //Display sensor values as percentage
  spr.drawString("%", 245, 190);

  // when the (Light, Temperature or Humidity) reaches (40 g or 40%), it warns us with the Motor
  if ( ( h < 30 || ServoAction == 1 ) && ServoSenal == false ) {
    Serial.println("IF 1, < 30, a 0 grados ");
    myservo.write(StatusServo);
    StatusServo -= 20;
    ServoAction = 1;

    if (StatusServo <= 0) {
      ServoSenal = true;
      ServoAction = 0;
    }

  }

  // when the (Light, Temperature or Humidity) falls below (40 g or 40%), the motor returns to its original position
  if ( ( h > 30 || ServoAction == 2 )  && ServoSenal == true) {

    Serial.println("IF 2, > 30, a 180 grados ");
    myservo.write(StatusServo);
    StatusServo += 20;
    ServoAction = 2;

    //
    if (StatusServo >= 180) {
      ServoSenal = false;
      ServoAction = 0;
    }

  }

  spr.pushSprite(0, 0); //Push to LCD
  delay(50);

}