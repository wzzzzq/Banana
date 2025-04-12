#include <Servo.h>                // 声明调用Servo.h库
#define  SERVO_NUM  6             //舵机数量.0...........................................................3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333
Servo myservo[SERVO_NUM];         //创建六个舵机类
const  byte servo_pin[SERVO_NUM] = {10, A2, A3, A0, A1, 7}; //宏定义舵机控制引脚

int data[10]={1500,0,1500,1200}, cnt, l, num;

int ServoPwmDuty[8] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; //PWM脉冲宽度
int ServoPwmDutySet[8] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; //PWM脉冲宽度
double ServoPwmDutyInc[8];   //为了速度控制，当PWM脉宽发生变化时，每2.5ms或20ms递增的PWM脉宽

double cx = 93, cy = 61, Kpx = 0.2, Kpy = 0.4,  xnow, ynow;
double angle[10] = {1500, 500, 500, 500, 1500, 1200}; 
double position[5][10]= {{500,500,500}, // 默认
                        {2500, 1000, 2000}, //抓球
                        {1900, 600, 2000}, //放木块
                        {1200,1000,1000}, //投球
                        {2500, 1200, 1600}}; //抓木块

bool ServoPwmDutyHaveChange = 0;  //脉宽有变化标志位[1500,1900, 600, 2000, 1500,1200]
bool found = 0;

int ServoTime = 5000;      //舵机从当前角度运动到指定角度的时间，也就是控制速度

unsigned long t, T;

void ServoSetPluseAndTime(int id, int p, int time)
{
  if (id >= 0 && id < 6 && p >= 500 && p <= 2500)
  {
    if (time < 20)
      time = 20;
    if (time > 30000)
      time = 30000;
    ServoPwmDutySet[id] = p;
    ServoTime = time;
    ServoPwmDutyHaveChange = 1;
  }
}

void ServoPwmDutyCompare(void)//脉宽变化比较及速度控制
{
  int i;

  static int ServoPwmDutyIncTimes; //需要递增的次数
  static bool ServoRunning = 0; //舵机正在以指定速度运动到指定的脉宽对应的位置
  if (ServoPwmDutyHaveChange) //停止运动并且脉宽发生变化时才进行计算      ServoRunning == 0 &&
  {
    ServoPwmDutyHaveChange = 0;
    ServoPwmDutyIncTimes = ServoTime / 20; //当每20ms调用一次ServoPwmDutyCompare()函数时用此句
    for (i = 0; i < 6; i++)
    {
      //if(ServoPwmDuty[i] != ServoPwmDutySet[i])
      {
        if (ServoPwmDutySet[i] > ServoPwmDuty[i])
        {
          ServoPwmDutyInc[i] = ServoPwmDutySet[i] - ServoPwmDuty[i];
          ServoPwmDutyInc[i] = -ServoPwmDutyInc[i];
        }
        else
        {
          ServoPwmDutyInc[i] = ServoPwmDuty[i] - ServoPwmDutySet[i];
        }
        ServoPwmDutyInc[i] /= ServoPwmDutyIncTimes;//每次递增的脉宽
      }
    }
    ServoRunning = 1;  //舵机开始动作
  }
  if (ServoRunning)
  {
    ServoPwmDutyIncTimes--;
    for (i = 0; i < 6; i++)
    {
      if (ServoPwmDutyIncTimes == 0)
      { //最后一次递增就直接将设定值赋给当前值
        ServoPwmDuty[i] = ServoPwmDutySet[i];
        ServoRunning = 0; //到达设定位置，舵机停止运动
      }
      else
      {
        ServoPwmDuty[i] = ServoPwmDutySet[i] + (signed short int)(ServoPwmDutyInc[i] * ServoPwmDutyIncTimes);
      }
      myservo[i].writeMicroseconds(ServoPwmDuty[i]);
    }
  }
}

void update_state(){
  angle[0] = data[0];
  int index=data[1];
  for (int i = 1; i < 4; i++)
  {
    angle[i] = position[index][i-1];
  }
  angle[4] = data[2];
  angle[5] = data[3];
  //for(int i=0;i<6;i++) angle[i]=data[i];
}

void GetData()
{
  found = 0;
  if (Serial.available())
  {
    found = 1;
    char input = Serial.read();
    while (input != '[' && input != -1)   input = Serial.read();
    if (input != -1)
    {
      String s = Serial.readStringUntil(']'), numstr = "";
      l = s.length(), cnt = 0;
      for (int i = 0; i < l; i++)
      {
        if (isDigit(s[i]))
          numstr += s[i];
        if (s[i] == ',' || i == l - 1)
        {
          data[cnt++] = numstr.toInt();
          numstr = "";
        }
      }
    }
    update_state();
  }
}

void PrintData()
{
  for (int i = 0; i < cnt; i++)
  {
    Serial.println(data[i]);
  }
  Serial.println("==========");
}

void setup() {
  //put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, INPUT);
  for (byte i = 0; i < SERVO_NUM; i++ ) {
    myservo[i].attach(servo_pin[i]);    // 将10引脚与声明的舵机对象连接起来
  }
  t = millis() + 20;
}

void loop() {
  //put your main code here, to run repeatedly:
  GetData();
  PrintData();
  //if (millis() > t)
  {
    for (int i = 0; i < 6; i++)  ServoSetPluseAndTime(i, angle[i], 60);
    ServoPwmDutyCompare();
    t = millis() + 20;
  }
}
