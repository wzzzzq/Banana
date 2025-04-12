#include "include.h"

const int L = 1010, R = 1040;
int mafor, mago, maturn;
int mbfor, mbgo, mbturn;
int mcfor, mcgo, mcturn;
int mdfor, mdgo, mdturn;
int speed_max = 255;
int speed_min = -255;
double ref1, ref2, ref3, ref4;
unsigned long t, T;

void TaskRun(void)
{
  if (millis() > t)
  {
    Write();
    GetSignal();
    t =  millis() + 30;
  }
  graph();
  if (rcsig.failsafe)
    fail_safe();
  else
  {
    if ((rcsig.ch[2]<800) || ((rcsig.ch[0] > L) && (rcsig.ch[0] < R) && (rcsig.ch[1] > L) && (rcsig.ch[1] < R) && (rcsig.ch[3] > L) && (rcsig.ch[3] < R))){
      stop_all();
      Serial.println("stop");
    }
    else
    {
      if (!((rcsig.ch[1] > L) && (rcsig.ch[1] < R)))
      {
        mafor = map(rcsig.ch[1], 340, 1704, speed_min, speed_max);
        mbfor = map(rcsig.ch[1], 340, 1704, speed_min, speed_max);
        mcfor = map(rcsig.ch[1], 340, 1704, speed_min, speed_max);
        mdfor = map(rcsig.ch[1], 340, 1704, speed_min, speed_max);
      }
      else
      {
        mafor = 0;
        mbfor = 0;
        mcfor = 0;
        mdfor = 0;
      }
      if (!((rcsig.ch[0] > L) && (rcsig.ch[0] < R)))
      {
        mago = map(rcsig.ch[0], 340, 1704, speed_min, speed_max);
        mbgo = map(rcsig.ch[0], 340, 1704, speed_max, speed_min);
        mcgo = map(rcsig.ch[0], 340, 1704, speed_min, speed_max);
        mdgo = map(rcsig.ch[0], 340, 1704, speed_max, speed_min);
      }
      else
      {
        mago = 0;
        mbgo = 0;
        mcgo = 0;
        mdgo = 0;
      }
      if (!((rcsig.ch[3] > L) && (rcsig.ch[3] < R)))
      {
        maturn = map(rcsig.ch[3], 340, 1704, speed_min, speed_max);
        mbturn = map(rcsig.ch[3], 340, 1704, speed_min, speed_max);
        mcturn = map(rcsig.ch[3], 340, 1704, speed_max, speed_min);
        mdturn = map(rcsig.ch[3], 340, 1704, speed_max, speed_min);
      }
      else
      {
        maturn = 0;
        mbturn = 0;
        mcturn = 0;
        mdturn = 0;
      }
      ref1 = max(min(mafor + mago + maturn, speed_max), speed_min);
      ref2 = max(min(mbfor + mbgo + mbturn, speed_max), speed_min);
      ref3 = max(min(mcfor + mcgo + mcturn, speed_max), speed_min);
      ref4 = max(min(mdfor + mdgo + mdturn, speed_max), speed_min);
      Serial.println(ref1);
      Serial.println(ref2);
      Serial.println(ref3);
      Serial.println(ref4);
      setspeed();
      moving();
    }
  }

}
