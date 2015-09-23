# 8051programs
For starters, I am uploading a program I used for my mini project.

// Program to make a digital clock with time and alarm setting features


#include<reg51.h>
sbit dig_ctrl_4=P1^0; //Declare the control pins for the seven segments
sbit dig_ctrl_3=P1^1;
sbit dig_ctrl_2=P1^2;
sbit dig_ctrl_1=P1^3;
sbit relaycontrol=P3^7;
sbit resetalarm=P1^5; //Reset alarm pin to reset the alarm.
sbit resetclock=P1^4; //Reset clock pin to reset the clock.
sbit start=P3^3; // Start pin for starting the clock after time setting
sbit incr=P1^7; // Increment pin to increase the digits during time setting.
sbit set=P1^6; // Set pin to set the time.
int sel_seg_to_incr=0;
int ret_seg_to_incre=0;
int recnt_incr_seg;
int begin;
unsigned char dig_disp=0;
int min2=0;
int min1=0;
int sec2=0;
int sec1=0;

int hour1=0;
int hour2=0;
int DP=0;

int houralarm2=0;
int houralarm1=0;
int minalarm2=0;
int minalarm1=0;
int alarmhour2=0;
int alarmhour1=0;
int alarmmin2=0;
int alarmmin1=0;
int keephour2=0;
int keephour1=0;
int keepmin2=0;
int keepmin1=0;
int loop=0;
int mode;
char dig_val[10]={0xC0,0xF9,0xA4,0xB0,0x99, 0x92,0x82,0xF8,0x80,0x90};
char dig_valdp[10]={0x40,0x79,0x24,0x30,0x19, 0x12,0x02,0x78,0x00,0x10};
char timerL1=0xc6;
char timerH1=0x3c;
int afterstart=1;


void delay(int time) //Function to provide time delay.
{
    int i,j;
    for(i=0;i<time;i++)
    for(j=0;j<1275;j++);
}

int setfn() // Function to select miniute and seconds digit set time.
{
    while(set==0)
    {
        switch(recnt_incr_seg)
        {
            
            case 1:
            if(set==0) //Select the min2 digit
            {
                dig_ctrl_4=1;
                dig_ctrl_3=0;
                dig_ctrl_2=0;
                dig_ctrl_1=0;
                recnt_incr_seg=1;
                ret_seg_to_incre=1;
                P2=dig_val[houralarm2];
                delay(100);
            }
            
            case 2:
            if(set==0) //Select the min1 digit
            {
                dig_ctrl_4=0;
                dig_ctrl_3=1;
                dig_ctrl_2=0;
                dig_ctrl_1=0;
                recnt_incr_seg=2;
                ret_seg_to_incre=2;
                P2=dig_val[houralarm1];
                delay(100);
            }
            
            case 3:
            if(set==0) //Select the sec 2 digit
            {
                dig_ctrl_4=0;
                dig_ctrl_3=0;
                dig_ctrl_2=1;
                dig_ctrl_1=0;
                recnt_incr_seg=3;
                ret_seg_to_incre=3;
                P2=dig_val[minalarm2];
                delay(100);
            }
            
            case 4:
            if(set==0) //Select the sec1 digit
            {
                recnt_incr_seg=1;
                dig_ctrl_4=0;
                dig_ctrl_3=0;
                dig_ctrl_2=0;
                dig_ctrl_1=1;
                ret_seg_to_incre=4;
                P2=dig_val[minalarm1];
                delay(100);
                recnt_incr_seg=1;
            }
        }
    }
    return(ret_seg_to_incre);
}

void increase(int a) //Function to set the minutes or seconds digit
{
    while(incr==0)
    {
        switch(a)
        {
            
            case 1: // Set the min2 digit.
            P2=dig_val[houralarm2];
            delay(100);
            houralarm2++;
            if(houralarm2==3)
            houralarm2=0;
            P2=dig_val[houralarm2];
            delay(20);
            break;
            
            case 2: //Set the min1 digit.
            P2=dig_val[houralarm1];
            delay(100);
            houralarm1++;
            if(houralarm1==10)
            houralarm1=0;
            P2=dig_val[houralarm1];
            delay(20);
            break;
            
            case 3: // Set the sec2 digit.
            P2=dig_val[minalarm2];
            delay(100);
            minalarm2++;
            if(minalarm2==6)
            minalarm2=0;
            P2=dig_val[minalarm2];
            delay(20);
            break;
            
            case 4: //Set the sec1 digit.
            //recnt_incr_seg=4;
            P2=dig_val[minalarm1];
            delay(100);
            minalarm1++;
            if(minalarm1==10)
            minalarm1=0;
            P2=dig_val[minalarm1];
            delay(20);
            break;
        }
    }
}

void resetfn(mode) // Function to bring the clock to reset or set mode.
{
    begin=1;
    dig_ctrl_4=1; //Enable the min2 digit and disable others
    dig_ctrl_3=0;
    dig_ctrl_2=0;
    dig_ctrl_1=0;
    if(mode==0) //Check if clock is in set alarm mode
    {
        IE=0x88; //Disable Timer0 interrupt to stop the display of clock.
        sel_seg_to_incr=1;
        recnt_incr_seg=1;
        P2=dig_val[keephour2];
        delay(100);
        houralarm2=keephour2;
        houralarm1=keephour1;
        minalarm2=keepmin2;
        minalarm1=keepmin1;
    }
    if(mode==1) //Check if clock is in set clock mode
    {
        IE=0x80; //Disable Timer0 interrupt to stop the clock.
        houralarm2=hour2;
        houralarm1=hour1;
        minalarm2=min2;
        minalarm1=min1;
        sel_seg_to_incr=1;
        recnt_incr_seg=1;
        P2=dig_val[hour2];
        delay(100);
    }
    while(1)
    {
        if(start==0) //Check if start pin is pressed
        {
            if(mode==0)
            {
                keephour2=houralarm2;
                keephour1=houralarm1;
                keepmin2=minalarm2;
                keepmin1=minalarm1;
                alarmhour2=houralarm2;
                alarmhour1=houralarm1;
                alarmmin2=minalarm2;
                alarmmin1=minalarm1;
            }
            if(mode==1)
            {
                hour2=houralarm2;
                hour1=houralarm1;
                min2=minalarm2;
                min1=minalarm1;
            }
            TMOD=0x11; //Reset the timer0
            TL0=0xf6;
            TH0=0xFf;
            IE=0x8A; //Enabling Timer0 interrupt to start the display of clock
            TR0=1;
            break;
        }
        if(set==0) //Check if set pin is pressed
        sel_seg_to_incr=setfn();
        if(incr==0) //Check if incr pin is pressed
        increase(sel_seg_to_incr);
    }
}

void display() interrupt 1 // Function to display the digits on seven segment using the concept of seven segment multiplexing.
{
    TL0=0x36; //Reload Timer0
    TH0=0xf6;
    P2=0xFF;
    dig_ctrl_1 = dig_ctrl_3 = dig_ctrl_2 = dig_ctrl_4 = 0;
    dig_disp++;
    dig_disp=dig_disp%4;
    switch(dig_disp)
    {
        case 0:
        if(hour1==0 && hour2==0)
        P2=dig_val[sec1];
        else
        P2=dig_val[min1];
        dig_ctrl_1 = 1;
        break;
        
        case 1:
        if(hour1==0 && hour2==0)
        P2=dig_val[sec2];
        else
        P2= dig_val[min2];
        dig_ctrl_2 = 1;
        break;
        
        case 2:
        if(hour1==0 && hour2==0)
        {
            if(DP)
            P2= dig_valdp[min1];
            else
            P2= dig_val[min1];
        }
        else
        {
            if(DP)
            P2= dig_valdp[hour1];
            else
            P2= dig_val[hour1];
        }
        dig_ctrl_3 = 1;
        break;
        
        case 3:
        if(hour1==0 && hour2==0)
        P2=dig_val[min2];
        else
        P2= dig_val[hour2];
        dig_ctrl_4 = 1;
        break;
    }
}

void moveclock() interrupt 3 // Function to increment clock digits
{
    TL1=timerL1;
    TH1=timerH1;
    loop++;
    
    if(loop==10)
    {
        DP=1-DP;
        if(DP==0)
        {
            sec1++;
            if(sec1==10)
            {
                sec1=0;
                sec2++;
                if(sec2==6)
                {
                    sec1=0;
                    sec2=0;
                    min1++;
                    if(min1==10)
                    {
                        sec1=0;
                        sec2=0;
                        min1=0;
                        min2++;
                        if(min2==6)
                        {
                            sec1=0;
                            sec2=0;
                            min1=0;
                            min2=0;
                            hour1++;
                            if(hour2==2 && hour1==4)
                            {
                                sec1=0;
                                sec2=0;
                                min1=0;
                                min2=0;
                                hour1=0;
                                hour2=0;
                            }
                            else if(hour1==10)
                            {
                                sec1=0;
                                sec2=0;
                                min1=0;
                                min2=0;
                                hour1=0;
                                hour2++;
                            }
                        }
                    }
                }
            }
        }
        loop=0;
    }
    
    if(sec1==0 && sec2==1 && min1==0 && min2==0 && afterstart==1)
    {
        sec1=0;
        sec2=0;
        min1=0;
        min2=0;
        afterstart=0;
    }
}

void main()
{
    mode=0;
    set=1; //Initialize set, reset, start and incr pins as input
    resetalarm=1;
    resetclock=1;
    start=1;
    incr=1;
    begin=0;
    TMOD=0x11; //Intialize Timer 0
    TL0=0xf6; //Load timer0
    TH0=0xFf;
    IE=0x8A; //Enable Timer0 interrupt
    TR0=1; //Start Timer0
    TL1=timerL1;
    TH1=timerH1;
    TR1=1; // Start Timer1
    relaycontrol=0;
    while(1)
    {
        
        if(resetalarm==0) //Check if reset alarm pin is pressed
        {
            resetfn(0);
        }
        if(resetclock==0)//Check if reset clock pin is pressed
        {
            resetfn(1);
        }
        if(hour2==alarmhour2&&hour1==alarmhour1&&min2==alarmmin2&&min1==alarmmin1&&begin==1)// Check for Alarm condition
        {
            relaycontrol=1;
            delay(3000);
            relaycontrol=0;
            delay(7000);
        }
    }
}
