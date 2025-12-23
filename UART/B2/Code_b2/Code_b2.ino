#define BUTTON 2          // PIN BUTTON
#define LED 3             // PIN LED
long boardrate = 9600;    // Boardrate mac dinh
unsigned long curTime = 0;  // thoi gian hien tai
const int holdTime = 3000;  // thoi gian cho mac dinh
int chedo = 0;
bool isPress = 0;
bool settingState = 0;
void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  Serial.begin(boardrate);
  Serial.println("BoardRate: " + String(boardrate));
}

void loop() {
  int btnState = digitalRead(BUTTON);
  if (btnState == LOW)
  {
    if (!isPress)
    {
      isPress = 1;
      curTime = millis();
    }
    if (millis() - curTime >= holdTime)
    {
      if (settingState == 0)
      {
        settingState = 1;
        Serial.println("--- Che do hieu chinh ---");
        curTime = millis();
        for (int i = 0; i < 4; i++)
        {
          digitalWrite(LED, 0);
          delay(50);
          digitalWrite(LED, 1);
          delay(50);
        }
      }
      else
      {
        settingState = 0;
        Serial.println("--- Thoat khoi che do hieu chinh ---");
        curTime = millis();
        for (int i = 0; i < 4; i++)
        {
          digitalWrite(LED, 0);
          delay(50);
          digitalWrite(LED, 1);
          delay(50);
        }
      }
      isPress = 0;
    }
  }
  else
  {
    if (isPress == 1)
    {
      if (settingState)
      {
        chedo += 1;
        if (chedo > 1)
          chedo = 0;
        switch (chedo)
        {
          case 0:
          {
            boardrate = 9600;
            Serial.println("Dang chuyen doi board rate sang " + String(boardrate));
            // Serial.end();
            // delay(100);
            // Serial.begin(boardrate);
            digitalWrite(LED, 0);
            delay(300);
            digitalWrite(LED, 1);
            delay(300);
            Serial.println("Da chuyen doi sang " + String(boardrate));
            break;
          }
          case 1:
          {
            boardrate = 115200;
            Serial.println("Dang chuyen doi board rate sang " + String(boardrate));
            // Serial.end();
            // delay(100);
            // Serial.begin(boardrate);
            for (int i = 0; i < 2; i++)
            {
              digitalWrite(LED, 0);
              delay(150);
              digitalWrite(LED, 1);
              delay(150);
            }
            Serial.println("Da chuyen doi sang " + String(boardrate));
            break;
          }
        }
      }
    }
    isPress = 0;
  }
  delay(100);
}
