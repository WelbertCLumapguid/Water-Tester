#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


float resolution;                                
int measurings;
float voltage;
float pHvalue;
float b = 0.00;
float m = 0.167;
float td;

void setup()                                     
{   
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);                            
  resolution = 1024.0;                                                                       
  delay(100);                                   
}


void loop()                                      
{
  measurings=0;                                  
  for (int i = 0; i < 10; i++)                   
  {
    measurings = measurings + analogRead(A0);     
    delay(10);                                  
  }
    voltage = ((5 / resolution) * (measurings/10)); 
                                                   
    pHvalue = ((7 + ((2.5 - voltage) / m)))+ b; 

    td = analogRead(A1) * 5.0/1024.0;

    Serial.print(pHvalue);
    Serial.print(",");
    Serial.println(td);                                                        

    lcd.setCursor(0, 0);
    lcd.print("pH Value = ");
    lcd.setCursor(12, 0);
    lcd.print(pHvalue);
    lcd.setCursor(0, 1);
    lcd.print("Tbdty = ");
    if (td>=4){
      lcd.setCursor(14,1);
      lcd.print("  ");
      lcd.setCursor(9, 1);
      lcd.print("clear");

    }
    else if (td<4 && td>=3){
      lcd.setCursor(14,1);
      lcd.print("  ");
      lcd.setCursor(9, 1);
      lcd.print("cloudy");

    }  
    else if (td<3 && td>=2){
      lcd.setCursor(14,1);
      lcd.print("  ");
      lcd.setCursor(9, 1);
      lcd.print("dirty");

    }  
    else if (td<2){
      lcd.setCursor(9, 1);
      lcd.print("V dirty");

    }                          
    delay(900);
    

                                     
}
