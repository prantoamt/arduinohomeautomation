#include <IRremoteInt.h>
#include <IRremote.h>

#include "pitches.h"
#include <LiquidCrystal.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"


int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

int relay1 = A0;
int relay2 = A1;
int relay3 = A3;
int relay4 = A2;
int relay5 = 8;
int relay6 = 9;

int buzzer = 7;

int relay_status[] = {0, 0, 0, 0, 0, 0};

int on = 0;
int off = 1;

char command;

byte temp[8] = {
  B01110,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
};

byte heart[8] = {
  B01010,
  B11111,
  B11111,
  B01110,
  B01110,
  B00100,
  B00000,
};


int RECV_PIN = 6;

IRrecv irrecv(RECV_PIN);
decode_results results;

#define code1 5;
#define code2  6;
#define code3  7;
#define code4  8;
#define code5  9;
#define code6  10;

int current_hour = 0;

void setup() {  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Wire.begin();
  rtc.begin();
  DateTime now = rtc.now();
  current_hour = now.hour();
  irrecv.enableIRIn();
  Serial.begin(9600);
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);

  pinMode(buzzer, OUTPUT);

  digitalWrite(relay1, off);
  digitalWrite(relay2, off);
  digitalWrite(relay3, off);
  digitalWrite(relay4, off);
  digitalWrite(relay5, off);
  digitalWrite(relay6, off);
  
  lcd.createChar(0, temp);
  lcd.createChar(1, heart);
  lcd.clear();
  welcomemsg();
  playMelody();

  lcd.clear();
}

uint32_t old_ts;

void loop() {
  
  checkRemote();
  checkCommand();
  printTime();
 
}


void printTime()
{
  DateTime now = rtc.now(); //get the current date-time
  uint32_t ts = now.getEpoch();

  int hours = now.hour();
  
  if(hours > 12)
  {
    hours = hours - 12; //sets the time is 12 hrs format
  }
  
  if (old_ts == 0 || old_ts != ts) {
      old_ts = ts;
  if(current_hour == now.hour())
  {
     lcd.setCursor(0,0);
     lcd.print("Time: ");
     lcd.print(hours, DEC);
     lcd.print(':');
     lcd.print(now.minute(), DEC);
     lcd.print(':');
     lcd.print(now.second(), DEC);
     lcd.setCursor(0,1);
     lcd.print("Date: ");
     lcd.print(now.date(), DEC);
     lcd.print('/');
     lcd.print(now.month(), DEC);
     lcd.print('/');
     lcd.print(now.year(), DEC);          
  }
  else if(current_hour != now.hour())
  {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Time: ");
     lcd.print(hours, DEC);
     lcd.print(':');
     lcd.print(now.minute(), DEC);
     lcd.print(':');
     lcd.print(now.second(), DEC);
     lcd.setCursor(0,1);
     lcd.print(now.date(), DEC);
     lcd.print('/');
     lcd.print(now.month(), DEC);
     lcd.print('/');
     lcd.print(now.year(), DEC);     
     lcd.print(' ');
     //lcd.print(weekDay[now.dayOfWeek()]);

     for(int  i = 0; i < hours; i++)
     {
       tone(buzzer, 900, 250);

       delay(1000);

       noTone(buzzer);
     }
     current_hour = now.hour();
  }
  
    }
  delay(1000);
    
}


void checkCommand()
{
   if(Serial.available() > 0)
  {
    command = Serial.read();

     switch (command)
  {
    case 'A':
        beep();
        if( relay_status[0] == 0)
        {
          digitalWrite(relay1, on);
          relay_status[0] = 1;
        }
        break;

    case 'a':
        beep();
        if(relay_status[0] == 1)
        {
          delay(1000);
          digitalWrite(relay1, off);
          relay_status[0] = 0;
        }
        break;    

    case 'B':
        beep();
        if( relay_status[1] == 0)
        {
          digitalWrite(relay2, on);
          relay_status[1] = 1;
        }
        break;

        
    case 'b':
        beep();
        if(relay_status[1] == 1)
        {
          delay(1000);
          digitalWrite(relay2, off);
          relay_status[1] = 0;          
        }
        break;

    case 'C':
        beep();
        if( relay_status[2] == 0)
        {
          digitalWrite(relay3, on);
          relay_status[2] = 1;          
        }
        break;

        
    case 'c':
        beep();
        if(relay_status[2] == 1)
        {
          delay(1000);
          digitalWrite(relay3, off);
          relay_status[2] = 0;          
        }
        break;

    case 'D':
        beep();
        if( relay_status[3] == 0)
        {
          digitalWrite(relay4, on);
          relay_status[3] = 1;          
        }
        break;

        
    case 'd':
        beep();
        if(relay_status[3] == 1)
        {
          delay(1000);
          digitalWrite(relay4, off);
          relay_status[3] = 0;         
        }
        break;

    case 'E':
        beep();
        if( relay_status[4] == 0)
        {
          digitalWrite(relay5, on);
          relay_status[4] = 1;          
        }
        break;

        
    case 'e':
        beep();
        if(relay_status[4] == 1)
        {
          delay(1000);
          digitalWrite(relay5, off);
          relay_status[4] = 0;       
        }
        break;

    case 'F':
        beep();
        if( relay_status[5] == 0)
        {
          digitalWrite(relay6, on);
          relay_status[5] = 1;          
        }
        break;

        
    case 'f':
        beep();
        if(relay_status[5] == 1)
        {
          delay(1000);
          digitalWrite(relay6, off);
          relay_status[5] = 0;        
        }
        break;
        
    } // end switch
    
  }//end if
  
}// end of function

void checkRemote()
{
  if (irrecv.decode(&results))
  {
    long int rslt = results.value;

     switch (rslt)
  {
    case 1086134503:
        beep();
        if( relay_status[0] == 0)
        {
          digitalWrite(relay1, on);
          relay_status[0] = 1;
        }
        else
        {
          delay(1000);
          digitalWrite(relay1, off);
          relay_status[0] = 0;
        }
        break;

    case 1086189583:
        beep();
        if( relay_status[1] == 0)
        {
          digitalWrite(relay2, on);
          relay_status[1] = 1;
        }
        else
        {
          delay(1000);
          digitalWrite(relay2, off);
          relay_status[1] = 0;
         }
        break;

    case 1086154903:
        beep();
        if( relay_status[2] == 0)
        {
          digitalWrite(relay3, on);
          relay_status[2] = 1;
        }
        else
        {
          delay(1000);
          digitalWrite(relay3, off);
          relay_status[2] = 0;
        }
        break;

    }//end switch
    delay(1000);
    irrecv.resume();
    
  }//end if
}


void playMelody()
{
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzer);
  }
}

void welcomemsg()
{
    lcd.clear();
    for(int i = 0; i < 4; i++)
    {
      lcd.setCursor(i,0);
      lcd.write(byte(1));
      delay(200);
    }

    lcd.print("Welcome!");

    for(int i = 12; i < 16; i++)
    {
      lcd.setCursor(i,0);
      lcd.write(byte(1));
      delay(200);
    }
    lcd.setCursor(0,1);

    lcd.print("System is ready!");
}


void beep()
{
  tone(buzzer, 900, 100);
  delay(200);
  noTone(buzzer);
}

