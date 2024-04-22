#include "mbed.h"
#include "WS2812.h"
#include "PixelArray.h"
#include "SongPlayer.h"
#include  "Speaker.h"
//#include "mbed2/240/TARGET_LPC1768/TARGET_NXP/TARGET_LPC176X/TARGET_MBED_LPC1768/PinNames.h"

#define WS2812_BUF 16
#define NUM_COLORS 6
#define NUM_LEDS_PER_COLOR 1

PixelArray px(WS2812_BUF);
DigitalIn SpeakerOn(p12);
Speaker mySpeaker(p18);
//SongPlayer mySpeaker((p26));
// See the program page for information on the timing numbers
// The given numbers are for the K64F
WS2812 ws(p9, WS2812_BUF, 3, 11, 10, 11);

int main()
{
//mySpeaker.PlaySong(note,duration,0.05);
    //  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==


    // loops forever playing two notes on speaker using analog samples

        




    ws.useII(WS2812::PER_PIXEL); // use per-pixel intensity scaling
    
    // set up the colours we want to draw with
    //int colorbuf[NUM_COLORS] = {0x2f0000,0x2f2f00,0x002f00,0x002f2f,0x00002f,0x2f002f};;
    //{0x2f0000,0x2f2f00,0x002f00,0x002f2f,0x00002f,0x2f002f};
    // for each of the colours (j) write out 10 of them
    // the pixels are written at the colour*10, plus the colour position
    // all modulus 60 so it wraps around
    //for (int i = 0; i < WS2812_BUF; i++) {
    //    px.Set(i, colorbuf[(i / NUM_LEDS_PER_COLOR) % NUM_COLORS]);
    //}
    
     for (int i = 0; i < WS2812_BUF; i++) {
        px.Set(i, 0xff0000);
    }
    // now all the colours are computed, add a fade effect using intensity scaling
    // compute and write the II value for each pixel
    for (int j=0; j<WS2812_BUF; j++) {
        //px.SetI(pixel position, II value)
        //px.SetI(j%WS2812_BUF, 0xf+(0xf*(j%NUM_LEDS_PER_COLOR)));
        px.SetI(j%WS2812_BUF, 50);
    }

    ws.write(px.getBuf());
    while (1) {
        //for (int z=WS2812_BUF; z >= 0 ; z--) {
           //ws.write_offsets(px.getBuf(),z,z,z);
           // wait(0.075);
        //}
    //float d = 0; //duration    
        









        


    for(float d = 1.01; d>0; d-=0.1)
    {

        if(d>0.09)
        {
            for (int j=0; j<WS2812_BUF; j++) {
                px.SetI(j%WS2812_BUF, 50);
            }
            ws.write(px.getBuf());
            mySpeaker.PlayNote(969.0, d, 0.05);
            for (int j=0; j<WS2812_BUF; j++) {
                px.SetI(j%WS2812_BUF, 0);
            }
            ws.write(px.getBuf());
            mySpeaker.PlayNote(800.0, d, 0.05);

        }
        while(d<0.09){
            while(1){
                for (int j=0; j<WS2812_BUF; j++) {
                    px.SetI(j%WS2812_BUF, 50);
                }
                ws.write(px.getBuf());
                mySpeaker.PlayNote(969.0, 10, 0.1);
            }
        }

    }

    

  
  
        
    }

}
