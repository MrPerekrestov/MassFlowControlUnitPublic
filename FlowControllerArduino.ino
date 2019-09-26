#include <EEPROM.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
int MenuPin = 4;
int NextPin = 3;
int ChangePin = 2;
int ChannelOneSet = A0;
int ChannelTwoSet = A1;
int ChannelThreeSet = A2;
int ChannelOneFlow = A3;
int ChannelTwoFlow = A4;
int ChannelThreeFlow = A5;
unsigned long ChannelOneSetValues = 0;
int ChannelOneSetCount = 0;
unsigned long ChannelTwoSetValues = 0;
int ChannelTwoSetCount = 0;
unsigned long ChannelThreeSetValues = 0;
int ChannelThreeSetCount = 0;
unsigned long ChannelOneFlowValues = 0;
int ChannelOneFlowCount = 0;
unsigned long ChannelTwoFlowValues = 0;
int ChannelTwoFlowCount = 0;
unsigned long ChannelThreeFlowValues = 0;
int ChannelThreeFlowCount = 0;
int NextValue = 0;
int CursorRow = 1;
int CursorDigit = 2;
int OldCursorRow = 1;
int OldCursorDigit = 2;
unsigned long Timer = 0;
boolean MenuStatus = false;
int i=0;
String MillisToStr = "";
String TempStr = "";
int ThirdDigitMillis = 0;
struct GasType // Gas parameters
 {
  String Name;
  float Coefficient;  
 };
 
struct FlowController // Flow controller parameters
 {
  int GasIndex; 
  int MaxFlowRateIndex;
  float SetPoint;
  float Flow;
 };

 //Table of commonly used gases and coefficients and length of the table
 
GasType GasTable[] = 
 {
  {"Ar " , 1.39},
  {"N2 " , 1.00},
  {"O2 " , 0.99},
  {"N-H" , 1.00},
  {"NH3" , 0.77},
  {"H2 " , 1.01},
  {"He " , 1.45},
  {"Ne " , 1.46},
  {"Xe " , 1.32},
  {"CO " , 1.00},
  {"CO2" , 0.74},
  {"H2S" , 0.82},
  {"NO " , 0.97},
  {"N2O" , 0.71}  
 };
int GasTableLength = (int)sizeof(GasTable)/sizeof(GasType)-1;

 //Table of commonly used max gas flow rates for MF1 MKS flow controllers and length of the table
int MaxFlowRateTable[] = {10,20,50,100,200,500};
int MaxFlowRateTableLength = (int)sizeof(MaxFlowRateTable)/sizeof(int)-1;

FlowController ChannelOne; //Define three flow controllers
FlowController ChannelTwo;
FlowController ChannelThree;
//--------------------------------------------------------------------------------------------------------------------------------
void setup() 
 {
  lcd.begin(16, 4);
  
  pinMode(MenuPin,INPUT);
  pinMode(NextPin,INPUT);
  pinMode(ChangePin, INPUT);
  pinMode(ChannelOneSet,INPUT);
  pinMode(ChannelTwoSet,INPUT);
  pinMode(ChannelThreeSet,INPUT);
  pinMode(ChannelOneFlow,INPUT);
  pinMode(ChannelTwoFlow,INPUT);
  pinMode(ChannelThreeFlow,INPUT);
  
 // attachInterrupt(digitalPinToInterrupt(NextPin), NextButtonCheck,RISING);
  
   //Setup flow controllers gases
  ChannelOne.GasIndex   =   EEPROM.read(0);
  ChannelTwo.GasIndex   =   EEPROM.read(1);
  ChannelThree.GasIndex =   EEPROM.read(2);
    
 //Setup max flow rates   
  ChannelOne.MaxFlowRateIndex   =   EEPROM.read(3); 
  ChannelTwo.MaxFlowRateIndex   =   EEPROM.read(4);
  ChannelThree.MaxFlowRateIndex =   EEPROM.read(5);

  /*ChannelOne.SetPoint = 26.7;
  ChannelTwo.SetPoint = 4.7;
  ChannelThree.SetPoint = 106;*/
  
 /* ChannelOne.Flow = 26.7;
  ChannelTwo.Flow = 4.7;
  ChannelThree.Flow = 106;*/
  
  lcd.setCursor(0, 0); lcd.print("****************");
  lcd.setCursor(4, 1); lcd.print("MF-1 MKS ");
  lcd.setCursor(2, 2); lcd.print("CONTROL UNIT");
  lcd.setCursor(2, 3); lcd.print("VERSION 1.0");
  delay(5000);
  lcd.clear();
  //  EEPROM.write(0,21);
 
 }
//----------------------------------------------------------------------------------------------------------------------------------
void loop() 
 {

  if (MenuStatus)
  {
  lcd.clear();
  while (MenuStatus)
    {     
      Menu();
    }
  }
  else
  {
    lcd.clear();
    while(!MenuStatus)
    {
      Monitor();
    }
  }   

 }



 
 void Menu()
 {  
  lcd.setCursor(0, 0);
  lcd.print("   CH1  CH2  CH3");
  
  lcd.setCursor(0, 1); lcd.print("GS");
  lcd.setCursor(3, 1); lcd.print(GasTable[ChannelOne.GasIndex].Name);
  lcd.setCursor(8, 1); lcd.print(GasTable[ChannelTwo.GasIndex].Name);
  lcd.setCursor(13, 1); lcd.print(GasTable[ChannelThree.GasIndex].Name);
  
 // lcd.setCursor(0, 2); lcd.print("CF ");
  lcd.setCursor(2, 2); lcd.print((String)GasTable[ChannelOne.GasIndex].Coefficient+" ");
  lcd.setCursor(7, 2); lcd.print((String)GasTable[ChannelTwo.GasIndex].Coefficient+" ");
  lcd.setCursor(12, 2); lcd.print((String)GasTable[ChannelThree.GasIndex].Coefficient+" ");
  
  lcd.setCursor(0, 3); lcd.print("MX");
  lcd.setCursor(3, 3); 
  TempStr = (String)MaxFlowRateTable[ChannelOne.MaxFlowRateIndex];
  if (TempStr.length() == 2) 
  {
    lcd.print(TempStr+" ");
  }
  else
  {
    lcd.print(TempStr);
  } 
  lcd.setCursor(8, 3); 
  TempStr = (String)MaxFlowRateTable[ChannelTwo.MaxFlowRateIndex];
  if (TempStr.length() == 2) 
  {
    lcd.print(TempStr+" ");
  }
  else
  {
    lcd.print(TempStr);
  }  
  lcd.setCursor(13, 3);
  TempStr = (String)MaxFlowRateTable[ChannelThree.MaxFlowRateIndex];
  if (TempStr.length() == 2) 
  {
    lcd.print(TempStr+" ");
  }
  else
  {
    lcd.print(TempStr);
  }    
  ChangeButtonCheck();
  MenuButtonCheck();
  NextButtonCheck();
  Cursor();      
}



void Monitor()
{
ChannelOneSetValues +=analogRead(ChannelOneSet);
ChannelOneSetCount++;
ChannelTwoSetValues +=analogRead(ChannelTwoSet);
ChannelTwoSetCount++;
ChannelThreeSetValues +=analogRead(ChannelThreeSet);
ChannelThreeSetCount++;
ChannelOneFlowValues+=analogRead(ChannelOneFlow);
ChannelOneFlowCount++;
ChannelTwoFlowValues+=analogRead(ChannelTwoFlow);
ChannelTwoFlowCount++;
ChannelThreeFlowValues+=analogRead(ChannelThreeFlow);
ChannelThreeFlowCount++;
 if ((millis()-Timer)>350)
  {
  Timer = millis();  
  
  lcd.setCursor(0, 0);
  lcd.print("   CH1  CH2  CH3");

  lcd.setCursor(0, 1); lcd.print("GS");
  lcd.setCursor(3, 1); lcd.print(GasTable[ChannelOne.GasIndex].Name);
  lcd.setCursor(8, 1); lcd.print(GasTable[ChannelTwo.GasIndex].Name);
  lcd.setCursor(13, 1); lcd.print(GasTable[ChannelThree.GasIndex].Name);
//--------------------------------------------------------------------------------------
  lcd.setCursor(0, 2); lcd.print("S");

  //CH1 SP
 
  // ChannelOne.SetPoint = analogRead(ChannelOneSet)/GasTable[ChannelOne.GasIndex].Coefficient*(MaxFlowRateTable[ChannelOne.MaxFlowRateIndex]/1024);
  //Read SP values
  ChannelOne.SetPoint = ChannelOneSetValues/ChannelOneSetCount;
  ChannelOneSetValues = 0;
  ChannelOneSetCount = 0;
  ChannelOne.SetPoint = ChannelOne.SetPoint*GasTable[ChannelOne.GasIndex].Coefficient;
  ChannelOne.SetPoint = ChannelOne.SetPoint/1024;
  ChannelOne.SetPoint = ChannelOne.SetPoint*MaxFlowRateTable[ChannelOne.MaxFlowRateIndex];

  ChannelTwo.SetPoint = ChannelTwoSetValues/ChannelTwoSetCount;
  ChannelTwoSetValues = 0;
  ChannelTwoSetCount = 0;
   ChannelTwo.SetPoint = ChannelTwo.SetPoint*GasTable[ChannelTwo.GasIndex].Coefficient;
  ChannelTwo.SetPoint = ChannelTwo.SetPoint/1024;
  ChannelTwo.SetPoint = ChannelTwo.SetPoint*MaxFlowRateTable[ChannelTwo.MaxFlowRateIndex];

  ChannelThree.SetPoint = ChannelThreeSetValues/ChannelThreeSetCount;
  ChannelThreeSetValues = 0;
  ChannelThreeSetCount = 0;
  ChannelThree.SetPoint = ChannelThree.SetPoint*GasTable[ChannelThree.GasIndex].Coefficient;
  ChannelThree.SetPoint = ChannelThree.SetPoint/1024;
  ChannelThree.SetPoint = ChannelThree.SetPoint*MaxFlowRateTable[ChannelThree.MaxFlowRateIndex];

  //Read Flow values
  ChannelOne.Flow = ChannelOneFlowValues/ChannelOneFlowCount;
  ChannelOneFlowValues = 0;
  ChannelOneFlowCount = 0;
  ChannelOne.Flow = ChannelOne.Flow*GasTable[ChannelOne.GasIndex].Coefficient;
  ChannelOne.Flow = ChannelOne.Flow/1024;
  ChannelOne.Flow = ChannelOne.Flow*MaxFlowRateTable[ChannelOne.MaxFlowRateIndex];
  
  ChannelTwo.Flow = ChannelTwoFlowValues/ChannelTwoFlowCount;
  ChannelTwoFlowValues = 0;
  ChannelTwoFlowCount = 0;
  ChannelTwo.Flow = ChannelTwo.Flow*GasTable[ChannelTwo.GasIndex].Coefficient;
  ChannelTwo.Flow = ChannelTwo.Flow/1024;
  ChannelTwo.Flow = ChannelTwo.Flow*MaxFlowRateTable[ChannelTwo.MaxFlowRateIndex];
  
  ChannelThree.Flow = ChannelThreeFlowValues/ChannelThreeFlowCount;
  ChannelThreeFlowValues = 0;
  ChannelThreeFlowCount = 0;
  ChannelThree.Flow = ChannelThree.Flow*GasTable[ChannelThree.GasIndex].Coefficient;   
  ChannelThree.Flow = ChannelThree.Flow/1024;
  ChannelThree.Flow = ChannelThree.Flow*MaxFlowRateTable[ChannelThree.MaxFlowRateIndex];
  if (ChannelOne.SetPoint<10)
  {
    lcd.setCursor(2, 2); lcd.print(ChannelOne.SetPoint,2);
  }
  else if (ChannelOne.SetPoint<100)
  {
    lcd.setCursor(2, 2); lcd.print(ChannelOne.SetPoint,1);
  }
  else
  {
    lcd.setCursor(2, 2); lcd.print(ChannelOne.SetPoint,0); lcd.print(" ");
  }
  //CH2 SP
   if (ChannelTwo.SetPoint<10)
  {
    lcd.setCursor(7, 2); lcd.print(ChannelTwo.SetPoint,2);
  }
  else if (ChannelTwo.SetPoint<100)
  {
    lcd.setCursor(7, 2); lcd.print(ChannelTwo.SetPoint,1);
  }
  else
  {
    lcd.setCursor(7, 2); lcd.print(ChannelTwo.SetPoint,0); lcd.print(" ");
  }
  //CH3 SP
   if (ChannelThree.SetPoint<10)
  {
    lcd.setCursor(12, 2); lcd.print(ChannelThree.SetPoint,2);
  }
  else if (ChannelThree.SetPoint<100)
  {
    lcd.setCursor(12, 2); lcd.print(ChannelThree.SetPoint,1);
  }
  else
  {
    lcd.setCursor(12, 2); lcd.print(ChannelThree.SetPoint,0); lcd.print(" ");
  }
//------------------------------------------------------------------------
  lcd.setCursor(0, 3); lcd.print("F");
  //CH1 FL
  if (ChannelOne.Flow<10)
  {
    lcd.setCursor(2, 3); lcd.print(ChannelOne.Flow,2);
  }
  else if (ChannelOne.Flow<100)
  {
    lcd.setCursor(2, 3); lcd.print(ChannelOne.Flow,1);
  }
  else
  {
    lcd.setCursor(2, 3); lcd.print(ChannelOne.Flow,0); lcd.print(" ");
  }
  //CH2 FL
   if (ChannelTwo.Flow<10)
  {
    lcd.setCursor(7, 3); lcd.print(ChannelTwo.Flow,2);
  }
  else if (ChannelTwo.Flow<100)
  {
    lcd.setCursor(7, 3); lcd.print(ChannelTwo.Flow,1);
  }
  else
  {
    lcd.setCursor(7, 3); lcd.print(ChannelTwo.SetPoint,0); lcd.print(" ");
  }
  //CH3 FL
   if (ChannelThree.Flow<10)
  {
    lcd.setCursor(12, 3); lcd.print(ChannelThree.Flow,2);
  }
  else if (ChannelThree.Flow<100)
  {
    lcd.setCursor(12, 3); lcd.print(ChannelThree.Flow,1);
  }
  else
  {
    lcd.setCursor(12, 3); lcd.print(ChannelThree.Flow,0); lcd.print(" ");
  }
 } 

  MenuButtonCheck();  
}



void MenuButtonCheck()
{
  if (digitalRead(MenuPin) == true)
  {
    delay(10);
    if (digitalRead(MenuPin) == true)
    {
      MenuStatus=!MenuStatus;
      delay(500);
    }    
  }
}


void NextButtonCheck()
{
  if (digitalRead(NextPin))
  {
    delay(10);  
    if (digitalRead(NextPin))
    {
      
      if (NextValue < 5)
      {
        NextValue++;
      }
      else
      {
        NextValue = 0;
      }
      switch (NextValue)
      {
        case 0: 
          CursorDigit = 2;
          CursorRow = 1;
          break;
        case 1:
          CursorDigit = 7;
          CursorRow = 1;
          break;
        case 2:
          CursorDigit = 12;
          CursorRow = 1;
          break;
        case 3:
          CursorDigit = 2;
          CursorRow = 3;
          break;
        case 4:
          CursorDigit = 7;
          CursorRow = 3;
          break;
        case 5:
          CursorDigit = 12;
          CursorRow = 3;
          break;
      }
      delay(300);
    }
  }  
}


void Cursor()
{
  if ((CursorDigit!=OldCursorDigit)||(CursorRow!=OldCursorRow))
  {
    lcd.setCursor(OldCursorDigit, OldCursorRow);
    lcd.print(" ");
    OldCursorDigit = CursorDigit;
    OldCursorRow = CursorRow;
  }
  MillisToStr = String(millis(), DEC);
  if (MillisToStr.length()>=3)
  {  
    MillisToStr = MillisToStr[MillisToStr.length()-3];
    ThirdDigitMillis = MillisToStr.toInt();    
    if (ThirdDigitMillis>4) 
    {
      lcd.setCursor(CursorDigit, CursorRow);
      lcd.print(" ");
    }
    else
    {
      lcd.setCursor(CursorDigit, CursorRow);
      lcd.print(">");
    }
  }
}



void ChangeButtonCheck()
{
  if (digitalRead(ChangePin))  
  {
    delay(10);
    if (digitalRead(ChangePin))  
    {
        switch (NextValue)
        {
          case 0:
            if (ChannelOne.GasIndex<GasTableLength)
            {
              ChannelOne.GasIndex++;
            }
            else 
            {
              ChannelOne.GasIndex=0;
            }
            EEPROM.write(0,ChannelOne.GasIndex);
            break;
          case 1:
            if (ChannelTwo.GasIndex<GasTableLength)
            {
              ChannelTwo.GasIndex++;
            }
            else 
            {
              ChannelTwo.GasIndex=0;
            }
            EEPROM.write(1,ChannelTwo.GasIndex);
            break;
          case 2:
            if (ChannelThree.GasIndex<GasTableLength)
            {
              ChannelThree.GasIndex++;
            }
            else 
            {
              ChannelThree.GasIndex=0;
            }
            EEPROM.write(2,ChannelThree.GasIndex);
            break;
          case 3:
            if (ChannelOne.MaxFlowRateIndex<MaxFlowRateTableLength)
            {
              ChannelOne.MaxFlowRateIndex++;
            }
            else 
            {
              ChannelOne.MaxFlowRateIndex=0;
            }
            EEPROM.write(3,ChannelOne.MaxFlowRateIndex);
            break;
          case 4:
            if (ChannelTwo.MaxFlowRateIndex<MaxFlowRateTableLength)
            {
              ChannelTwo.MaxFlowRateIndex++;
            }
            else 
            {
              ChannelTwo.MaxFlowRateIndex=0;
            }
            EEPROM.write(4,ChannelTwo.MaxFlowRateIndex);
            break;
          case 5:
            if (ChannelThree.MaxFlowRateIndex<MaxFlowRateTableLength)
            {
              ChannelThree.MaxFlowRateIndex++;
            }
            else 
            {
              ChannelThree.MaxFlowRateIndex=0;
            }
            EEPROM.write(5,ChannelThree.MaxFlowRateIndex);
            break;
        }
        delay(300);
    }
    
  }
}
