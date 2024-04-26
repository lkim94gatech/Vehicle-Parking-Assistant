#include "mbed.h"
#include "motordriver.h"
#include "ultrasonic.h"
#include "uLCD_4DGL.h"
#include  "Speaker.h"
#include "rtos.h"
#include "PixelArray.h"
#include "WS2812.h"

#define WS2812_BUF 16
#define NUM_COLORS 6
#define NUM_LEDS_PER_COLOR 1


BusOut myled(LED1,LED2,LED3,LED4);
Serial blue(p28,p27);
Motor  left(p21, p22, p23, 1); // pwm, fwd, rev, has brake feature
Motor right(p26, p25, p24, 1);
ultrasonic mu(p6, p7, .1, 1, NULL);
ultrasonic mu1(p12, p13, .1, 1, NULL);
uLCD_4DGL uLCD(p9, p10, p11);
Speaker mySpeaker(p18);
WS2812 ws(p20, WS2812_BUF, 3, 11, 10, 11);
PixelArray px(WS2812_BUF);
void alertDisplay(int distance);

void light_on(){
    for (int j=0; j<WS2812_BUF; j++) {
                    px.SetI(j%WS2812_BUF, 50);
                }
                ws.write(px.getBuf());
}

void light_off(){
    for (int j=0; j<WS2812_BUF; j++) {
                    px.SetI(j%WS2812_BUF, 0);
                }
                ws.write(px.getBuf());
}
void light_color(int color){
    ws.useII(WS2812::PER_PIXEL); //this may be required to change colors i dont know.
    for (int i = 0; i < WS2812_BUF; i++) { //change color for loop 
        px.Set(i, color);
    }
}
void rainbow(){
//              LINK TO LIBRARY AND DOCUMENTATION
//-----=====https://os.mbed.com/users/bridadan/code/WS2812/shortlog/=====-----
int colorbuf[NUM_COLORS] = {0x2f0000,0x2f2f00,0x002f00,0x002f2f,0x00002f,0x2f002f}; //different LED colors to rotate through
for (int i = 0; i < WS2812_BUF; i++) {
    px.Set(i, colorbuf[(i / NUM_LEDS_PER_COLOR) % NUM_COLORS]);
}
for (int j=0; j<WS2812_BUF; j++) {
    // px.SetI(pixel position, II value)
    px.SetI(j%WS2812_BUF, 100); //the second value is the brightness i do not know the range, but 255 caused weird things to happen
    //px.SetI(j%WS2812_BUF, 0xf+(0xf*(j%NUM_LEDS_PER_COLOR))); //previous code line before i replaced it with 100 
}
// Now the buffer is written, rotate it
// by writing it out with an increasing offset
for (int z=WS2812_BUF; z >= 0 ; z--) {
    ws.write_offsets(px.getBuf(),z,z,z); //this command does not like interrupts
    wait(0.075);
}

}

void beep(){
    float d = 1;
    mySpeaker.PlayNote(1200.0, d, 0.5);
}

void updateDisplay(int distance) {
    uLCD.cls();
    uLCD.background_color(BLACK);
    uLCD.locate(0, 1);
    uLCD.color(WHITE);
    uLCD.printf("Distance: %d mm   ", distance);
}


void alertDisplay(int distance) {
    uLCD.cls();  // Clear the screen for the alert
    uLCD.background_color(RED);  // Set the background to red
    uLCD.text_width(2);  // Make the text larger
    uLCD.text_height(2);
    uLCD.color(WHITE);
    uLCD.locate(0, 1);
    uLCD.printf("STOP! ");
    uLCD.locate(0, 3);
    uLCD.printf("Dist: %dmm", distance);
}


void checkDistance() {
    mu.startUpdates(); // Start ultrasonic updates
    mu.checkDistance(); // Check for updates
    mu.startUpdates(); // Start ultrasonic updates
    mu.checkDistance(); // Check for updates
    int currentDistance = mu.getCurrentDistance();
    int currentDistance1 = mu1.getCurrentDistance();
    updateDisplay(currentDistance); // Update uLCD display with current distance
}

void stop() {
    left.speed(0.0);
    right.speed(0.0);
    wait(0.5);
}

void park() {
    left.speed(-0.3);
    right.speed(0.3);
    wait(0.7);
}

void park1() {
    left.speed(-0.5);
    right.speed(0.5);
    wait(0.2);
}

void ready() {
    left.speed(0.85);
    right.speed(0.4);
    wait(0.7);
}

void reverse() {
    left.speed(-0.3);
    right.speed(-0.3);
    while(1) {
        //change lcd ui
        checkDistance();
        int distance = mu.getCurrentDistance();
            if (distance > 500) {
            mySpeaker.PlayNote(800.0, 0.1, 0.3);
            light_on();
            mySpeaker.PlayNote(800.0, 0.1, 0.0);
            light_off();
            }
            if(distance > 75 && distance < 500){
                mySpeaker.PlayNote(800.0, 0.025, 0.3);
                light_on();
                mySpeaker.PlayNote(800.0, 0.025, 0.0);
                light_off();
            }
        if (distance < 75) {
            alertDisplay(distance);
            stop();
            break;
        }
    }
    park1();
}


int main()
{
    //LED CODE
    ws.useII(WS2812::PER_PIXEL); //this may be required to change colors i dont know.
    for (int i = 0; i < WS2812_BUF; i++) { //change color for loop 
        px.Set(i, 0xff0000);
    }
  
    //==    ==  ==  ==  ==  ==  ==  ==  ==  ==   ==  == ==  ==  ==  ==  ==  === ==  ==  ==  ==  ==
    char bnum=0;
    char bhit=0;

    uLCD.background_color(BLACK);
    uLCD.text_width(2); // Set text width to 2x
    uLCD.text_height(2); // Set text height to 2x
    uLCD.cls(); // Clear the screen

    mu.startUpdates();
    mu1.startUpdates();
    
    while(1) {
        
        checkDistance();
        if (blue.getc()=='!') {
            if (blue.getc()=='B') { //button data packet
                bnum = blue.getc(); //button number
                bhit = blue.getc(); //1=hit, 0=release
                if (blue.getc()==char(~('!' + 'B' + bnum + bhit))) { //checksum OK?
                    myled = bnum - '0'; //current button number will appear on LEDs
                    switch (bnum) {
                        case '1': //number button 1
                            if (bhit=='1') {
                                left.speed(-0.5);
                                right.speed(-0.5);

                                while(1) {
                                    //change lcd ui
                                    
                                    checkDistance();
                                    int distance = mu.getCurrentDistance();
                                     if (distance > 500) {
                                     mySpeaker.PlayNote(800.0, 0.1, 0.3);
                                     light_on();
                                     mySpeaker.PlayNote(800.0, 0.1, 0.0);
                                     light_off();
                                     }
                                     if(distance > 100 && distance < 500){
                                         mySpeaker.PlayNote(800.0, 0.025, 0.3);
                                         light_on();
                                         mySpeaker.PlayNote(800.0, 0.025, 0.0);
                                         light_off();
                                     }
                                    if (distance < 150) {
                                        alertDisplay(distance);
                                        stop();
                                        park();
                                        light_on();
                                        beep();
                                        break;
                                    }
                                }
                            } else {
                                stop();
                            }
                            break;
                        case '2': //number button 2
                            if (bhit=='1') {
                                park();
                            } else {
                                stop();
                            }
                            break;
                        case '3': //number button 3
                            if (bhit=='1') {
                                left.speed(0.4);
                                right.speed(0.4);

                                while (1) {
                                    checkDistance();
                                    int distance = mu1.getCurrentDistance();
                                    if (distance > 100) {
                                        alertDisplay(distance);
                                        ready();
                                        stop();
                                        reverse();
                                        park1();
                                        break;
                                    }
                                }
                            } else {
                                stop();
                            }
                            break;
                        case '4': //number button 4
                            if (bhit=='1') {
                                //add hit code here
                                rainbow(); //HERE IS WHERE THE RAINBOW FUNCTION IS, IT IS THE DEFAULT LIGHT OPTION XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                            } else {
                                stop();
                            }
                            break;
                        case '5': //button 5 up arrow
                            if (bhit=='1') {
                                left.speed(1.0);
                                right.speed(1.0);
                                wait(0.02);
                                light_off();
                            } else {
                                stop();
                            }
                            break;
                        case '6': //button 6 down arrow
                            if (bhit=='1') {
                                    //change lcd ui
                                checkDistance();
                                int distance = mu.getCurrentDistance();
                                    if (distance > 500) {
                                    mySpeaker.PlayNote(800.0, 0.1, 0.3);
                                    light_on();
                                    mySpeaker.PlayNote(800.0, 0.1, 0.0);
                                    light_off();
                                    }
                                    if(distance > 150 && distance < 500){
                                        mySpeaker.PlayNote(800.0, 0.025, 0.3);
                                        light_on();
                                        mySpeaker.PlayNote(800.0, 0.025, 0.0);
                                        light_off();
                                    }
                                left.speed(-0.5);
                                right.speed(-0.5);
                                wait(0.2);
                                break;
                                } else {
                                stop();
                            }
                            break;
                        case '7': //button 7 left arrow
                            if (bhit=='1') {
                                left.speed(0.5);
                                right.speed(1.0);
                                wait(0.02);
                            } else {
                                stop();
                            }
                            break;
                        case '8': //button 8 right arrow
                            if (bhit=='1') {
                                left.speed(1.0);
                                right.speed(0.5);
                                wait(0.02);
                            } else {
                                stop();
                            }
                            break;
                        default:
                            checkDistance();
                            break;
                    }
                }
            }
        }
        
    }
}
