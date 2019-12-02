#include <Timezone.h>

#define pin1 2 //+side for 1st - 4th candle
#define pin2 3
#define pin3 5
#define pin4 6
#define sD 50 //min on
#define lD 200 //max on
#define osf 4 //factor by which the off is shortan than the on

TimeChangeRule cest = {"CEST", Last, Sun, Mar, 2, +120};
TimeChangeRule cet = {"CET", Last, Sun, Oct, 3, +60};
Timezone centralEurope(cest, cet);
int y = 2000; //year
int p = 100; //1st Advent
int s = 100; //2nd Advent
int t = 100; //3rd Advent
int q = 100; //4th Advent
int n = 0; //Number of candles

void setup() {
  Serial.begin(115200);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  randomSeed(analogRead(0));
}

void loop() {
  if(Serial.available()){
      char serialInput = Serial.read();
      if (serialInput == 't') {
        unsigned long t = Serial.parseInt();
        setTime(t);
        y = year(centralEurope.toLocal(now()));
        int d = (y-2000)%28;
        int r = 24;
        r -= 5*(d/4) + d % 4;
        if(r<18){
          r += ((18-r)/7+1)*7;
        }
        q = r;
        t = r-7;
        s = r-14;
        p = r-21;
      }
    }
  int m = month(centralEurope.toLocal(now()));
  int d = day(centralEurope.toLocal(now()));
  n = 0;
  if(m==11&&p<1){
    if(d>=29+p){
      n = 1;
    }
  }else if(m==12&&p<1){
    n = 1;
  }else if(m==12&&p>0){
    if(d>=p){
      n = 1;
    }
  }
  if(m==12){
    if(d>=s){
      n = 2;
    }
    if(d>=t){
      n = 3;
    }
    if(d>=q){
      n = 4;
    }
  }
  flame(n);
}

void flame(int count){
  if (count == 0){
    return;
  }
  int pin = random(count);
  if (pin == 0) {
    pin = pin1;
  }else if (pin==1){
    pin = pin2;
  }else if (pin==2){
    pin = pin3;
  }else{
    pin = pin4;
  }
  digitalWrite(pin, LOW);
  delay(random(sD/osf, lD/osf));
  digitalWrite(pin, HIGH);
  delay(random(sD/count, lD/count));
}
