#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/delay.h>

#include "spilcd.h"
#include "AVR035.h"

#define LCD_DATA_PORT PORTC
#define LCD_DATA_PIN  6

#define LCD_SHIFT_PORT PORTB
#define LCD_SHIFT_PIN  0

#define LCD_LATCH_PORT PORTC
#define LCD_LATCH_PIN  7

/*
LCD pins connected to 74hc595:
Q7 ->LCD_D7
Q6 ->LCD_D6
Q5 ->LCD_D5
Q4 ->LCD_D4
Q3 ->RS
Q2 ->E
Q1 ->NC
Q0 ->NC
 */

uint8_t lcdpins = 0;

//init is special, first 3 commands only send 1 nibble
void SPILCD_init()
{
    LCD_shiftout(0x30);  //hippie says 00110011
    _delay_ms(10);
    LCD_shiftout(0x30);  //hippie says 00110010
    _delay_ms(1);
    LCD_shiftout(0x30);  //hippie says not needed
    _delay_ms(1);

    LCD_sendCommand(0b00101000);    //display format
    _delay_ms(1);
    LCD_sendCommand(0b00001100);    //display on
    _delay_ms(1);
    LCD_sendCommand(0b00000110);    //display move
    _delay_ms(1);
    LCD_sendCommand(0b00000001);    //clear screen
    _delay_ms(10);
    LCD_sendData('T');
    _delay_ms(10);
    LCD_sendData('e');
    _delay_ms(10);
    LCD_sendData('s');
    _delay_ms(10);
    LCD_sendData('t');
    _delay_ms(10);
}

//high nibble first
//pins on 595 is 7654RE--
void LCD_sendData(uint8_t byte) //RS HIGH
{
    //F.eks. 11000011
    uint8_t byte1 = byte & 0b11110000;  //11000000
    uint8_t byte2 = byte & 0b00001111;  //00000011
    byte2 = byte2 << 4;                //00110000

    byte1 = byte1 | 0b00001000; //set rs
    byte2 = byte2 | 0b00001000; //set rs

    LCD_shiftout(byte1);
    LCD_pulse_enable();
    LCD_shiftout(byte2);
    LCD_pulse_enable();
}

void LCD_sendCommand(uint8_t command) //RS LOW
{
    //F.eks. 11000011
    uint8_t byte1 = command & 0b11110000;  //11000000
    uint8_t byte2 = command & 0b00001111;  //00000011
    byte2 = byte2 << 4;               //00110000

    byte1 = byte1 & 0b11110111; //clear rs
    byte2 = byte2 & 0b11110111; //clear rs

    LCD_shiftout(byte1);
    LCD_pulse_enable();
    LCD_shiftout(byte2);
    LCD_pulse_enable();
}

void LCD_shiftout(uint8_t data)
{
    CLEARBIT(LCD_LATCH_PORT,LCD_LATCH_PIN);
    CLEARBIT(LCD_SHIFT_PORT,LCD_SHIFT_PIN);

    for (uint8_t i=0; i<8; i++)
    {
        if (GETBIT(data,i))
            SETBIT(LCD_DATA_PORT,LCD_DATA_PIN);
        else
            CLEARBIT(LCD_DATA_PORT,LCD_DATA_PIN);
        SETBIT(LCD_SHIFT_PORT,LCD_SHIFT_PIN);
        CLEARBIT(LCD_SHIFT_PORT,LCD_SHIFT_PIN);
    }

    SETBIT(LCD_LATCH_PORT,LCD_SHIFT_PIN);
    CLEARBIT(LCD_LATCH_PORT,LCD_SHIFT_PIN);

    lcdpins = data;
}

//maybe we can set enable together with the original shiftout ?...
void LCD_pulse_enable()
{
    //strobe E pin
    lcdpins = lcdpins | 0b00000100;
    LCD_shiftout(lcdpins);
    lcdpins = lcdpins & 0b11111011;
    LCD_shiftout(lcdpins);
}
