#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);

#define BLUE_RX 9
#define BLUE_TX 8
#define LCD_COLS 20
#define LCD_ROWS 4

SoftwareSerial BlueSerial(BLUE_TX,BLUE_RX);

void setup() {
  Serial.begin(9600);
  BlueSerial.begin(9600);
  lcd.begin(LCD_COLS,LCD_ROWS);
  lcd.print("Wireless");
  lcd.setCursor(0,1);
  lcd.print("Notice Board");

  delay(5000);
  lcd.clear();
  lcd.print("Configuring GSM");
  lcd.setCursor(0,1);
  lcd.print("1 of 2: ");
  Serial.print("AT+CMGF=1\r");
  delay(1000);
  if(Serial.available()<1)
  {
    lcd.print("Error!");
    while(1);
  }
  else lcd.print("OK");
  delay(1000);
  lcd.clear();
  lcd.print("Configuring GSM");
  lcd.setCursor(0,1);
  lcd.print("2 of 2: ");
  while(Serial.available()>0)Serial.read();
  Serial.print("AT+CNMI=2,2,0,0,0\r");
  delay(1000);
  if(Serial.available()<1)
  {
    lcd.print("Error!");
    while(1);
  }
  else lcd.print("OK");
  delay(1000);
  lcd.clear();
  lcd.print("READY");
}

void loop() {
  if(Serial.available()>0)
  {
    String Packet=Serial.readString();
    if(Packet.length()>5 && Packet.substring(2,7)=="+CMT:")
    {
      String From, unknownparameter, Time, Message;
      
      uint8_t startpos  = Packet.indexOf('"');
      uint8_t stoppos   = Packet.indexOf('"',startpos+1);
      From=Packet.substring(startpos+1,stoppos);
          
      startpos  = Packet.indexOf('"',stoppos+1);
      stoppos   = Packet.indexOf('"',startpos+1);
      unknownparameter = Packet.substring(startpos+1,stoppos);
  
      startpos  = Packet.indexOf('"',stoppos+1);
      stoppos   = Packet.indexOf('"',startpos+1);
      Time = Packet.substring(startpos+1,stoppos);
      
      Message = Packet.substring(Packet.indexOf('\n',7)+1);
      
      DisplayMessage(Time.substring(3,Time.indexOf('+'))+ "  "+Message);
    }
  }
  if(BlueSerial.available()>0)
  {
    String Packet=BlueSerial.readString();
    DisplayMessage(Packet);
  }
}

void DisplayMessage(String Msg)
{
   Msg.replace("\r\n","");
   Msg.replace("\n","");

   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(Msg.substring(0,LCD_COLS));
   lcd.setCursor(0,1);
   lcd.print(Msg.substring(LCD_COLS,2*LCD_COLS));
   lcd.setCursor(0,2);
   lcd.print(Msg.substring(2*LCD_COLS,3*LCD_COLS));
   lcd.setCursor(0,3);
   lcd.print(Msg.substring(3*LCD_COLS,4*LCD_COLS));
}

