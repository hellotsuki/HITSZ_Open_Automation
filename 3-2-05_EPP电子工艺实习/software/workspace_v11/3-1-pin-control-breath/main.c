/*
 * 呼吸灯
 */
#include <msp430.h>
void ClockInit()
{//最终MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ
     UCSCTL6 &= ~XT1OFF;       //启动XT1
     P5SEL |= BIT2 + BIT3;     //XT2引脚功能选择
     UCSCTL6 &= ~XT2OFF;       //打开XT2
     __bis_SR_register(SCG0);
     UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
     UCSCTL1 = DCORSEL_4;  //DCO频率范围在28.2MHZ以下
     UCSCTL2 = FLLD_5 + 1;       //D=16，N=1

     //n=8,FLLREFCLK时钟源为XT2CLK；
     //DCOCLK=D*(N+1)*(FLLREFCLK/n);
     //DCOCLKDIV=(N+1)*(FLLREFCLK/n);
     UCSCTL3 = SELREF_5 + FLLREFDIV_3;

     //ACLK的时钟源为DCOCLKDIV,
     //MCLK\SMCLK的时钟源为DCOCLK
     UCSCTL4 = SELA_4 + SELS_3 +SELM_3;

     //ACLK由DCOCLKDIV的32分频得到，
     //SMCLK由DCOCLK的2分频得到
     UCSCTL5 = DIVA_5 +DIVS_1;
}
unsigned int flag=0;
int main(void)
{
// stop watchdog timer
WDTCTL = WDTPW | WDTHOLD;

P1DIR&=~BIT3; //p1.3 input,pull up/s2
P1REN|=BIT3;
P1OUT|=BIT3;
P1IE|=BIT3; //enable P1.3 interrupt
P1IES|=BIT3; //high-low transition
P1IFG&=~BIT3;

P2DIR&=~BIT3; //p2.3 input,pull up/s3
P2REN|=BIT3;
P2OUT|=BIT3;
P2IE|=BIT3; //enable P2.3 interrupt
P2IES|=BIT3; //high-low transition
P2IFG&=~BIT3;

ClockInit();
// 选择时钟ACLK, 清除 TAR，增计数
TA0CTL = TASSEL_2 + TACLR + MC0;
TA0CCR0 = 1000;   // PWM周期
TA0CCTL1 = OUTMOD_7 ;// 输出模式7


_enable_interrupt();



}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{


if(P1IFG&BIT3){
flag=1;
unsigned int i;
for( i=1;i<=1e3;i++){

                    TA0CCR1 = i;  //占空比80%
                    P1DIR |= BIT2;    // P1.2 方向为输出
                    P1SEL |= BIT2;   // P1.2端口为外设，定时器TA0.1
                    __delay_cycles(1e4);
            }
}
P1IFG&=~BIT3;

}
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
if(P2IFG&BIT3){
flag=2;
unsigned int i;
for( i=1e3;i>=1;i--){

                        TA0CCR1 = i;  //占空比80%
                        P1DIR |= BIT2;    // P1.2 方向为输出
                        P1SEL |= BIT2;   // P1.2端口为外设，定时器TA0.1
                        __delay_cycles(1e4);
                }
}
P2IFG&=~BIT3;
}
