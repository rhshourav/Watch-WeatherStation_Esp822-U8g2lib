


const int selectBtn = 5;
const int upBtn = 12;
const int downBtn = 13;
const int leftBtn = 15;
const int rightBtn = 4;
const int delayMenu = 400;
const int exitBtn = 0;
bool exitTimer = false;
void resetFunction(){
  
const int selectBtn = 5;
const int upBtn = 12;
const int downBtn = 13;
const int leftBtn = 15;
const int rightBtn = 4;
const int delayMenu = 400;
const int exitBtn = 0;
bool exitTimer = false;
}
void weather(){
  Serial.println("==================Showing Weather=============================");
  resetFunction();
}
void countTimer(int hh, int mm, int ss){
  resetFunction();
  bool State = true;
  while(State){
    Serial.println("HH: " + String(hh) + " MM: " + String(mm) + " SS: " + String(ss));
  static unsigned long lastTime = 0; 
    unsigned long currentTime = millis();
    if(hh == 0 && mm == 0 && ss == 0){
      Serial.println("TImer finished...");
      Serial.println("HH: " + String(hh) + " MM: " + String(mm) + " SS: " + String(ss));
      State = false;
    }
    if (ss == 0 && mm != 0){
      ss = 59;
      mm--;
    }
    if (ss == 0 && mm == 0 && hh != 0){
      ss = 59;
      mm = 59;
      hh--;
    }
    
    if (currentTime - lastTime >= 1000) {
        lastTime = currentTime;  
        ss--;
    }
    if (digitalRead(exitBtn) == LOW){
      State = false;
      exitTimer = true;
    }
    
    
    
    delay(17);
    }
    
}
void timer(bool Status){
  resetFunction();
  bool loop = true;
  int setHMS = 1;
  int timerHH = 0;
  int timerMM = 0;
  int timerSS = 0;
  delay(400);

  while(Status){
    Serial.println("HH: " + String(timerHH) + " MM: " + String(timerMM) + " SS: " + String(timerSS));
    if( digitalRead(rightBtn) == HIGH){
      setHMS++;
      delay(delayMenu);
    }
    if(digitalRead(leftBtn) == HIGH){
      setHMS--;
      delay(delayMenu);
    }
    if(setHMS == 0 ){
      setHMS = 3;
    }
    if(setHMS > 3){
      setHMS = 1;
    }
    if (setHMS == 1 && digitalRead(upBtn) == HIGH){
      timerHH++;
      
      
      Serial.print("HH: ");
      Serial.println(timerHH);
      delay(100);
    }
    if (setHMS == 1 && digitalRead(downBtn) == HIGH){
      timerHH--;
      if (timerHH < 0){
        timerHH = 0;
      }
      Serial.print("HH: ");
      Serial.println(timerHH);
      delay(100);
    }
    if (setHMS == 2 && digitalRead(upBtn) == HIGH){
      timerMM++;
      if (timerMM > 59){
        timerMM = 0;
      }
      
      Serial.print("MM: ");
      Serial.println(timerMM);
      delay(100);
    }
    if (setHMS == 2 && digitalRead(downBtn) == HIGH){
      timerMM--;
      if (timerMM < 0){
        timerMM = 0;
      }
      Serial.print("MM: ");
      Serial.println(timerMM);
      delay(100);
    }
    if (setHMS == 3 && digitalRead(upBtn) == HIGH){
      timerSS++;
      if (timerSS > 59){
        timerSS = 0;
      }
      Serial.print("SS: ");
      Serial.println(timerSS);
      delay(100);
    }
    if (setHMS == 3 && digitalRead(downBtn) == HIGH){
      timerSS--;
      if (timerSS < 0){
        timerSS = 0;
      }
      Serial.print("SS: ");
      Serial.println(timerSS);
      delay(100);
    }
    
    if (digitalRead(selectBtn) == HIGH){
      
      Serial.println("HH: " + String(timerHH) + " MM: " + String(timerMM) + " SS: " + String(timerSS));
      Status = false;
      countTimer(timerHH, timerMM, timerSS);
    }
    if (digitalRead(exitBtn) == LOW){
      Status = false;
    }
    if (exitTimer == true){
      Status = false;
      exitTimer = false;
    }

    delay(17);
  }


}

void stopWatch(bool State){
  resetFunction();
  bool bol1 = false;
  bool bol2 = true ;
  ;
  while(State){///REMEMBER TO CHANGE THIS
    int h = 0 ;
    int m = 0 ;
    int s = 0 ;
    bool erdBool = true;
    bool ndBool = true;
    
    while(State && bol1 == true ){
      
      static unsigned long lastTime = 0; 
      unsigned long currentTime = millis();

    
      if (currentTime - lastTime >= 1000) {
          lastTime = currentTime; 
          if (s>= 59 ){
            m++;
            s = 0;
          }
          if (m >= 59){
            h++;
            m = 0;
          }


          Serial.println("HH: "+ String(h) + " mm: " + String(m) + " ss: " +String(s) );

          s++;
        
        }
        delay(100);
       if (digitalRead(selectBtn) == HIGH){
        State = false;
        }
        if (digitalRead(exitBtn) == LOW){
        ndBool  = false;
        State = false;
        erdBool = false;
    }
      }


    delay(500);
    
    if( bol2 == false){
    Serial.println("Result..");
    Serial.println("HH: "+ String(h) + " mm: " + String(m) + " ss: " +String(s) );
    }else{
      Serial.println("Prese X to Start...");
    }

    while(ndBool && bol2 == false){
      delay(20);
      if (digitalRead(selectBtn) == HIGH){
        ndBool = false;
        State = true;
        int h = 0 ;
        int m = 0 ;
        int s = 0 ;
        Serial.println(" Reset Done..");
        Serial.println("HH: "+ String(h) + " mm: " + String(m) + " ss: " +String(s) );
        Serial.println("Prese X to Start...");
        delay(delayMenu);
      }
      if (digitalRead(exitBtn) == LOW){
      ndBool  = false;
      State = false;
      erdBool = false;
    }
      
    }

    
    delay(500);
    while(erdBool){
      delay(20);
      if (digitalRead(selectBtn) == HIGH){
        State = true;
        bol1 = true;
        bol2 = false;
        erdBool = false;
        delay(delayMenu);
      }
      if (digitalRead(exitBtn) == LOW){
      ndBool  = false;
      State = false;
      erdBool = false;
    }

    }


  }
}
void handleReq(int reqNum){
  resetFunction();
  if (reqNum == 1){
    weather();
  }else if (reqNum == 2 ){
    timer(true);
  }else if (reqNum == 3){
    stopWatch(true);
  }else{
    Serial.print("Someting went Wrong.");
    
  }
}
void  menu(bool menuStatus){
  resetFunction();

  int i = 1;
  delay(400);
  while(menuStatus){
    
    if ( i == 1){
      Serial.println("1. Weather.");
    }
    if (i == 2){
      Serial.println("2. Timer.");
    }
    if (i == 3){
      Serial.println("3. Stop Watch.");
    }
    if (digitalRead(upBtn) == HIGH){
      i--;
      delay(delayMenu);
    }else if( digitalRead(downBtn) == HIGH){
      i++;
      delay(delayMenu);
    }
    if (i == 0 ){
      i = 3;
    }
    if ( i > 3){
      i = 1;
    }
    if (digitalRead(selectBtn) == HIGH){
      menuStatus = false;
      Serial.println(i);
      handleReq(i);
    }
    if (digitalRead(exitBtn) == LOW){
      menuStatus = false;
    }
  }
  delay(400);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(selectBtn, INPUT);
  pinMode(upBtn, INPUT);
  pinMode(downBtn, INPUT);
  pinMode(leftBtn, INPUT);
  pinMode(rightBtn, INPUT);
  pinMode(exitBtn, INPUT);
  Serial.println("Initing Program");
  
}

void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(selectBtn) == HIGH){
    menu(true);
    

  }
}
