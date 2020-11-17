#include <LiquidCrystal.h> //LCD library
#include <SoftwareSerial.h>
#include "DHT.h"
#define DHTPIN 11        // Where you have connected you DHT Sensor
#define DHTTYPE DHT11    // Defines the type of the sensor
//#define DHTTYPE DHT22    // Uncomment this line if you are using DHT22 sensor
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial sim(9, 10); //Rx, Tx
LiquidCrystal lcd(2,3,4,5,6,7);//RS, E, D4, D5, D6, D7
String number = "+254797277217";
bool red_alert = false;
float h;
int _timeout;
long duration, inches, cm;
const int pingPin = 12; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 13; // Echo Pin of Ultrasonic Sensor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sim.begin(9600); //Begins communication with the gsm
  dht.begin();// Begins the communication with the temperature sensor
  lcd.begin(16,2);
  Serial.println("System started"); 
   lcd.setCursor(1,0);
   lcd.print("Initializing");
   delay(1000);
   lcd.clear();


}

void loop() {
  // put your main code here, to run repeatedly:

  int count = 0;
    
  h = dht.readHumidity();         // reads the humidy and stores on the float variable named h
  //float t = dht.readTemperature();      // reads the temperature and stores on the float variable named t

  Serial.print("Humidity: ");           // prints the message Humidity: on the serial monitor
  Serial.print(h);                      // prints the humidity value on the serial monitor
  Serial.println();                     // goes to the next line on the serial monitor
 
  lcd.clear();                          // clears the LCD
  lcd.setCursor(0, 0);                  // sets the cursor on the first line
  lcd.print("Humidity:");               // prints the message Humidity: on the LCD
  lcd.setCursor(10, 0);                 // sets the cursor on the 10th column on the LCD
  lcd.print(h);                         // prints the humidity value on the LCD

  /*Serial.print("Temperature: ");        // prints the message Temperature: on the serial monitor 
  Serial.print(t);                      // prints the temperature value on the serial monitor
  Serial.println();                     // goes to the next line on the serial monitor

  lcd.setCursor(0, 1);                  //sets the cursor on the bottom line on the LCD
  lcd.print("Temperature:");            //prints the message Temperature on the LCD
  lcd.setCursor(13, 1);                 //sets the cursor on the 13 column on the bottom
  lcd.print(t); */                        //prints the temperature value on the lcd

  if (isnan(h)) {                    //This if statement check to see if the DHT sensor is connected or disconnected
    Serial.println("Sensor is not connected");   //prints the message sensor is not connected on the serial monitor  
    lcd.clear();                                 // clears the lcd
    lcd.setCursor(0, 1);                         // sets the cursor on the bottom on the lcd
    lcd.print("Disconnected");                   // prints Disconnected on the lcd
    return;                                      // return from the if statement
  }
  
    if(h > 60){
        Serial.print("Red Alert\n");
        lcd.clear();
        lcd.setCursor(3,1);
        lcd.print("It's foggy");
        while(red_alert  == false){
          while(count<1){
            redAlertOn();
            count++;
          }
          red_alert = true;
        }
    }

    else{
      Serial.println("No red alert");
      lcd.clear();
      lcd.setCursor(4,1);
      lcd.print("No fog"); 
        while(red_alert){
          while(count<1){
            redAlertOff();
            count++;
          }
          red_alert = false;
        }
      
    }
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   int dist = microsecondsToCentimeters(duration);

   if(dist < 50){
    Serial.println("Slow down, car ahead");
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("Car Ahead!!");
    carAhead();
   }
   delay(1000);
}


long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

   void redAlertOn()
    {
      Serial.println ("Sending Message........\n");
      sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);
      //Serial.println ("Set SMS Number"); nb
      sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
      delay(1000);
      //char value[5];
      //itoa(h,value,10); //convert integer to char array      
      String SMS = "Slow Down! It's foggy!";
      sim.println(SMS);
      Serial.println(SMS);
      delay(100);
      sim.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      //_buffer = _readSerial();
    }


   
    void redAlertOff()
    {
      Serial.println ("Sending Message..........\n");
      sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);
      //Serial.println ("Set SMS Number");
      sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
      delay(1000);
      //char value[5];
      //itoa(h,value,10); //convert integer to char array      
      String SMS = "Drive normally! No fog!";
      sim.println(SMS);
      Serial.println(SMS);
      delay(100);
      sim.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      //_buffer = _readSerial();
    }
    
    void carAhead()
    {
      Serial.println ("Sending Message..........\n");
      sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);
      //Serial.println ("Set SMS Number");
      sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
      delay(1000);
      //char value[5];
      //itoa(h,value,10); //convert integer to char array      
      String SMS = "Slow down! There is a car ahead";
      sim.println(SMS);
      Serial.println(SMS);
      delay(100);
      sim.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      //_buffer = _readSerial();
    }
    String _readSerial() {
      _timeout = 0;
      while(!sim.available() && _timeout < 12000  )
      {
        delay(13);
        _timeout++;
      }
      if (sim.available()) {
        return sim.readString();
        Serial.print("sms sent");
      }
    }
