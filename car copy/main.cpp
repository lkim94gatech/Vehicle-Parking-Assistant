#include "mbed.h"
#include "motordriver.h"
#include "ultrasonic.h"
#include "uLCD_4DGL.h"

BusOut myled(LED1,LED2,LED3,LED4);
Serial blue(p28,p27);
Motor  left(p21, p22, p23, 1); // pwm, fwd, rev, has brake feature
Motor right(p26, p25, p24, 1);
ultrasonic mu(p6, p7, .1, 1, NULL);
ultrasonic mu1(p12, p13, .1, 1, NULL);
uLCD_4DGL uLCD(p9, p10, p11);


void updateDisplay(int distance1, int distance2) {
    uLCD.locate(0, 1);
    uLCD.color(WHITE);
    uLCD.printf("Dist: %d mm   ", distance1);
    uLCD.locate(0, 2);
    uLCD.color(WHITE);
    uLCD.printf("Dist: %d mm   ", distance2);
}

void checkDistance() {
    mu.startUpdates(); // Start ultrasonic updates
    mu.checkDistance(); // Check for updates
    mu.startUpdates(); // Start ultrasonic updates
    mu.checkDistance(); // Check for updates
    int currentDistance = mu.getCurrentDistance();
    int currentDistance1 = mu1.getCurrentDistance();
    updateDisplay(currentDistance, currentDistance1); // Update uLCD display with current distance
}

void stop() {
    left.speed(0.0);
    right.speed(0.0);
    wait(0.5);
}

void park() {
    left.speed(-0.5);
    right.speed(0.5);
    wait(0.5);
}

void park1() {
    left.speed(-0.2);
    right.speed(0.2);
    wait(0.05);
}

void ready() {
    left.speed(0.75);
    right.speed(0.3);
    wait(1.0);
}

void reverse() {
    left.speed(-0.1);
    right.speed(-0.3);
    while(1) {
        //change lcd ui
        checkDistance();
        int distance = mu.getCurrentDistance();
        if (distance < 50) {
            stop();
            break;
        }
    }
    park();
}


int main()
{
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
                                    if (distance < 150) {
                                        stop();
                                        park();
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
                                left.speed(0.5);
                                right.speed(0.5);

                                while (1) {
                                    checkDistance();
                                    int distance = mu1.getCurrentDistance();
                                    if (distance > 100) {
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
                            } else {
                                stop();
                            }
                            break;
                        case '5': //button 5 up arrow
                            if (bhit=='1') {
                                left.speed(1.0);
                                right.speed(1.0);
                                wait(0.02);
                            } else {
                                stop();
                            }
                            break;
                        case '6': //button 6 down arrow
                            if (bhit=='1') {
                                left.speed(-1.0);
                                right.speed(-1.0);
                                wait(0.02);
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
