
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int trigPin = 13;
int echoPin = 10;
unsigned long openTime = 0;  // Store when the door opens
bool doorOpen = false;       // Track if the door is open

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.begin(16, 2);
  myservo.attach(9);

  Serial.begin(9600);
  myservo.write(0); // Ensure door starts in the closed position
}

void loop()
{
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.412;

  lcd.clear();

  if (distance > 20) 
  {

    lcd.setCursor(0, 0);
    lcd.print("WELCOME");
    lcd.setCursor(0, 1);
    lcd.print("DISTANCE: >20cm");

    if (!doorOpen)  // Open only if not already open
    {
      myservo.write(90);
      openTime = millis(); // Save the opening time
      doorOpen = true;
    }
  }
  else if (distance <= 20 && distance > 10) 
  {
    lcd.setCursor(0, 0);
    lcd.print("PLEASE ENTER");
    lcd.setCursor(0, 1);
    lcd.print("DISTANCE: ");
    lcd.print(distance);

    lcd.print("cm");
  }

  // Ensure door closes after 30 seconds
  if (doorOpen && (millis() - openTime >= 5000)) 
  {
    myservo.write(0);  // Move servo back to closed position
    delay(1000);       // Small delay to let it close
    doorOpen = false;  // Reset door state
  }

  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
}