#include <Wire.h>                        // i2C 통신을 위한 라이브러리
#include <LiquidCrystal_I2C.h>    
#include <string.h>
LiquidCrystal_I2C lcd(0x3f,16,2);     // 접근주소: 0x3F or 0x27
// triggerPin, EchoPin 지정
#define triggerPin 8
#define echoPin 9
 
void setup() {
    lcd.init();  
    lcd.backlight();   
    lcd.clear();
    
    Serial.print(lcd.status());
    pinMode(triggerPin, OUTPUT); 
    pinMode(echoPin, INPUT); 
    Serial.begin(9600); 
    delay(2000);
    Serial.println("Comport Connected..!");
    delay(500);
}

double Previous_dist = 0;
void loop() {
    double Duration;
    // Trigger High 지속 시간 10us
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10); 
    digitalWrite(triggerPin, LOW); 
 
    //echoPin High 유지 시간 측정
    Duration = pulseIn(echoPin, HIGH); 
    
    double Distance_mm = Duration / 5.8;
    double Distance_inch = Duration / 148;
 
    // 거리 측정 값이 유효할 때(2~400cm) 시리얼 포트로 거리를 출력
    if ((Distance_mm < 4000) && (Distance_mm > 2)){
        Serial.print("Distance = "); 
        Serial.print(Distance_mm);
        Serial.println(" mm");
        Serial.print("Distance = "); 
        Serial.print(Distance_inch);
        Serial.println(" inch");
        Serial.println("=================");
    }
    else{
        Serial.println("Error");
    }

    double diff = Previous_dist - Distance_mm;
    char buffer[15];
    dtostrf(diff,10,10,buffer);
    if((Previous_dist - Distance_mm) > 100 ){
      lcd.clear();
      lcd.setCursor(0,0);             // 1번째, 1라인
      lcd.print("I got something");
      lcd.setCursor(0,1);             // 1번째, 2라인
      lcd.print(buffer);
      Serial.println("=================");
    }
    Previous_dist = Distance_mm;
    delay(1000); 
}
