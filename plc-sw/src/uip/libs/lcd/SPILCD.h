#ifndef __spilcd_H
    #define __spilcd_H
    void LCD_pulse_enable(void);
    void SPILCD_init(void);
    void LCD_sendData(uint8_t byte);
    void LCD_sendCommand(uint8_t command);
    void LCD_shiftout(uint8_t data);
#endif