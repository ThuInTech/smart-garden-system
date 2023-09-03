#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
int motor = 2;
int fan = 13;
int led = 12;
int warningled  = 11;
int lightSensorPin = A2;
int LM35Pin = A1;
int soilMoisturePin = A0;
int rainSensor = 3;
int pHsensorPin = A3;
void setup()
{
  Serial.begin(9600);
  pinMode (soilMoisturePin, INPUT);
  pinMode (lightSensorPin, INPUT);
  pinMode (rainSensor, INPUT);
  pinMode (pHsensorPin, INPUT);
  pinMode (LM35Pin, INPUT);
  pinMode (warningled, OUTPUT);
  pinMode (motor, OUTPUT);
  pinMode (fan, OUTPUT);
  pinMode (led, OUTPUT);
  lcd.init();
  lcd.backlight(); 
 
}

void loop() {
  // Độ ẩm đất
  int soilMoistureValue = analogRead(soilMoisturePin);
  int Moisture = map(soilMoistureValue, 0, 1023, 0, 100);
  lcd.setCursor(0, 0);
  lcd.print("DO AM DAT: ");
  lcd.print(Moisture);
  lcd.print("%");
  if (Moisture < 30) {
    Serial.println("Motor: BAT");
    digitalWrite(motor, HIGH);
  }
  else
  {
    Serial.println("Motor: TAT");
    digitalWrite(motor, LOW);
  }
  // Nhiệt độ môi trường
  int LM35Value = analogRead(LM35Pin);
  float temperature = (LM35Value * 5.0 * 100.0) / 1023.0;
  lcd.setCursor(0, 1);
  lcd.print("NHIET DO: ");
  lcd.print(temperature);
  lcd.print("c");
  if (temperature > 30) {
    Serial.println("Fan: BAT");
    digitalWrite(fan, HIGH);
  }
  else 
  {
    Serial.println("Fan: TAT");
    digitalWrite(fan, LOW);
  }
  // Cảm biến ánh sáng tự động bật tắt đèn
  int lightValues = analogRead (lightSensorPin);
  Serial.print("ANH SANG:");
  Serial.print(lightValues);
  Serial.println(" lux");
  if (lightValues < 100)
  {
    Serial.println("Den: BAT");
    digitalWrite(led, HIGH);
  }
  else 
  {
    Serial.println("Den: TAT");
    digitalWrite(led, LOW);
  }
  // Cảm biến mưa
  int rainValues = digitalRead(rainSensor);
  if (rainValues == HIGH && digitalRead(motor) == HIGH) {
    Serial.println("Sap mua! Motor: TAT");
    digitalWrite(motor, LOW);
  }
  // Đo độ pH và cảnh báo nếu pH quá cao hoặc quá thấp
  int pHValue = analogRead(pHsensorPin);
  float pH = (float)pHValue / 1023.0 * 14.0;
  lcd.setCursor(0, 2);
  lcd.print("DO PH: ");
  lcd.print(pH, 2);

  if (pH < 5.5)
  {
    digitalWrite (warningled, HIGH);
    Serial.println("Do pH thap");
    Serial.println("Den canh bao bat");
  } 
  else if (pH > 7.5)
  {
    digitalWrite (warningled, HIGH);
    Serial.println("Do pH cao");
    Serial.println("Den canh bao bat");
  } 
  else 
  {
    digitalWrite (warningled, LOW);
    Serial.println("Do pH on dinh");
    Serial.println("Den canh bao tat");
  }
  delay (1000);
}