#include <LiquidCrystal.h>
#include <TimerOne.h>

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
bool request = false;

char value_bar_element[8] = 
{
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

void make_measure()
{
  float value = analogRead(A5) * 5.0 / 1023;
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (request)
    lcd.print("Request ");
  else
    lcd.print("Timer ");
  
  lcd.print(value);
  lcd.print(" V");
  
  lcd.setCursor(0, 1);
  for (int i = 0; i < value / 5 * 16; i++)
  {
    lcd.write(byte(0));
  }
}

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcd.createChar(0, value_bar_element);

  pinMode(2, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(A5, INPUT);
  
  Serial.begin(9600);

  Timer1.initialize(500000);
  Timer1.attachInterrupt(make_measure);
}

void loop()
{
  if (digitalRead(8) == LOW)
  {
    if (request)
    {
      detachInterrupt(digitalPinToInterrupt(2));
      Timer1.attachInterrupt(make_measure);
    }
    else
    {
      Timer1.detachInterrupt();
      attachInterrupt(digitalPinToInterrupt(2), make_measure, RISING);
    }

    request = !request;
    delay(200);
  }
}