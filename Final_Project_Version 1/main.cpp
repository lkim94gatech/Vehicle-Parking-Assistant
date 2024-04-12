#include "mbed.h"
#include "ultrasonic.h"
#include "Motor.h"

 void dist(int distance)
{
    //put code here to execute when the distance has changed
    printf("Distance %d mm\r\n", distance);
}

ultrasonic mu(p12, p13, .1, 1, &dist);    //Set the trigger pin to D8 and the echo pin to D9
                                        //have updates every .1 seconds and a timeout after 1
                                        //second, and call dist when the distance changes
Motor m(p23, p6, p5); // pwm, fwd, rev
AnalogIn mypotentiometer(p20);
RawSerial  pc(USBTX, USBRX);
RawSerial  dev(p28,p27);
DigitalOut led1(LED1);
DigitalOut led4(LED4);

void dev_recv()
{
    led1 = !led1;
    while(dev.readable()) {
        pc.putc(dev.getc());
    }
}

void pc_recv()
{
    led4 = !led4;
    while(pc.readable()) {
        dev.putc(pc.getc());
    }
}


int main()
{   
    double p = 0;
    p = (mypotentiometer*2) - 1;
    
    mu.startUpdates();//start measuring the distance

    pc.baud(9600);
    dev.baud(9600);
    pc.attach(&pc_recv, Serial::RxIrq);
    dev.attach(&dev_recv, Serial::RxIrq);

    while(1)
    {
        //Do something else here
        mu.checkDistance();     //call checkDistance() as much as possible, as this is where
                                //the class checks if dist needs to be called.
        p = (mypotentiometer*2) - 1;
        m.speed(p);
        wait(0.2);
       
    }
}
