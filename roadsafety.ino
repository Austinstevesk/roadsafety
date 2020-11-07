#include <LiquidCrystal.h> //LCD library
#include <SoftwareSerial.h>
SoftwareSerial sim(9, 10); //Rx, Tx
LiquidCrystal lcd(2,3,4,5,6,7);//RS, E, D4, D5, D6, D7
String number = "+2547.....";
int percentval;
bool red_alert = false;
int _timeout;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sim.begin(9600);
  lcd.begin(16,2);
  Serial.println("System started"); 
   lcd.setCursor(1,0);
   lcd.print("Initializing");
   delay(1000);
   lcd.clear();


}

void loop() {
  // put your main code here, to run repeatedly:

    int sensorval = analogRead(A0);

    Serial.print("\nAnalogValue: ");
    Serial.print(sensorval);
    percentval = map(sensorval, 1023, 200, 0, 100);
    Serial.print("\nPercentval: ");
    Serial.print(percentval);
    Serial.println(" ");
    //Serial.println(percentval);

    if(percentval > 30){
        Serial.print("Red Alert\n");
        redAlertOn();
        lcd.setCursor(3,1);
        lcd.print("Red alert");
        red_alert = true;   
    }

    else{
      Serial.println("No red alert");
      redAlertOff();
      lcd.setCursor(1,1);
      lcd.print("Red alert Off");     
      red_alert = false;
      
    }
    

  delay(1000);
  lcd.setCursor(6,0);
  lcd.print(percentval);

  
}



   void redAlertOn()
    {
      Serial.println ("Sending Message........\n");
      sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);
      //Serial.println ("Set SMS Number"); nb
      sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
      delay(1000);
      char value[5];
      itoa(percentval,value,10); //convert integer to char array      
      String SMS = "Red alert on!! Humidity value is "+String(value);
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
      char value[5];
      itoa(percentval,value,10); //convert integer to char array      
      String SMS = "Red alert off!! Humidity value is "+String(value);
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
