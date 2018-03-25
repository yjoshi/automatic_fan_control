#include <16F877A.h>
#device ADC=10                              //Setting ADC bits
#FUSES NOWDT               
#FUSES NOBROWNOUT          
#FUSES NOLVP                
#use delay(crystal=20000000)
#byte lcd=0x08
#byte TRIS_lcd=0x88
#bit rs=0x06.2
#bit en=0x06.3
#bit TRIS_rs=0x86.2
#bit TRIS_en=0x86.3
long int adc_value,real_value;        
int i;
char value[4];
unsigned char text[]="Temperature:";
void display(unsigned char a, int b);
void motor();
void main()
{
  TRIS_lcd=TRIS_rs=TRIS_en=0;
  display(0x38,0);
  display(0x01,0);
  display(0x0c,0);
    for(i=0;i<=12;i++)
    {
     display(text[i],1); 
    }
  setup_timer_2(T2_DIV_BY_16,255,1);    //Setting timers for PWM      
  SETUP_ADC_PORTS(AN0);                   //Setting ADC PORTS
  SET_ADC_CHANNEL(0);                       //Selecting Channel
  SETUP_ADC(ADC_CLOCK_INTERNAL);
     while(TRUE)
      {
        delay_us(20);
        adc_value=READ_ADC();            //Reading ADC value
        delay_us(10);
        real_value=adc_value/2;           //Obtaining real value
        motor();
        sprintf(value,"%lu",real_value);  //Changing int to char
        display(0x8c,0);
          for(i=0;i<=3;i++)
           {
             display(value[i],1);
           }   
      }
 }
void display(unsigned char a,int b)         //LCD sub routine
  {
    lcd=a;
    rs=b;
    en=1;
    delay_ms(10);
    en=0;
    delay_ms(10);
  }
 
void motor()
{
  if(real_value<10)
   {
   setup_ccp1(CCP_OFF);             
   }
  if(real_value>10)
   {
     setup_ccp1(CCP_PWM);
      if(real_value>=10&&real_value<=29)
      {
        set_pwm1_duty((int16)204);         //20% duty cycle PWM
      }
      else if(real_value>=30&&real_value<=69)
     {
       set_pwm1_duty((int16)510);         //50% duty cycle PWM 
      }
      else if(real_value>=70&&real_value<=99)
      {
         set_pwm1_duty((int16)715);      //70% duty cycle PWM
       }
      else if(real_value>=100&&real_value<=150)
       {
         set_pwm1_duty((int16)919);     //90% duty cycle PWM
       }
    }
 }
