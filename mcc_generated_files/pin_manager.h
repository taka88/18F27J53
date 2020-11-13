/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC18F27J53
        Driver Version    :  2.1.2
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.30 and above
        MPLAB 	          :  MPLAB X 5.40	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set USB_attach aliases
#define USB_attach_TRIS                 TRISAbits.TRISA0
#define USB_attach_LAT                  LATAbits.LATA0
#define USB_attach_PORT                 PORTAbits.RA0
#define USB_attach_ANS                  ANCON0bits.PCFG0
#define USB_attach_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define USB_attach_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define USB_attach_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define USB_attach_GetValue()           PORTAbits.RA0
#define USB_attach_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define USB_attach_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define USB_attach_SetAnalogMode()      do { ANCON0bits.PCFG0 = 1; } while(0)
#define USB_attach_SetDigitalMode()     do { ANCON0bits.PCFG0 = 0; } while(0)

// get/set IR_Receive aliases
#define IR_Receive_TRIS                 TRISAbits.TRISA1
#define IR_Receive_LAT                  LATAbits.LATA1
#define IR_Receive_PORT                 PORTAbits.RA1
#define IR_Receive_ANS                  ANCON0bits.PCFG1
#define IR_Receive_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define IR_Receive_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define IR_Receive_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define IR_Receive_GetValue()           PORTAbits.RA1
#define IR_Receive_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define IR_Receive_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define IR_Receive_SetAnalogMode()      do { ANCON0bits.PCFG1 = 1; } while(0)
#define IR_Receive_SetDigitalMode()     do { ANCON0bits.PCFG1 = 0; } while(0)

// get/set IR_Receive_LED aliases
#define IR_Receive_LED_TRIS                 TRISAbits.TRISA2
#define IR_Receive_LED_LAT                  LATAbits.LATA2
#define IR_Receive_LED_PORT                 PORTAbits.RA2
#define IR_Receive_LED_ANS                  ANCON0bits.PCFG2
#define IR_Receive_LED_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define IR_Receive_LED_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define IR_Receive_LED_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define IR_Receive_LED_GetValue()           PORTAbits.RA2
#define IR_Receive_LED_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define IR_Receive_LED_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define IR_Receive_LED_SetAnalogMode()      do { ANCON0bits.PCFG2 = 1; } while(0)
#define IR_Receive_LED_SetDigitalMode()     do { ANCON0bits.PCFG2 = 0; } while(0)

// get/set RA5 procedures
#define RA5_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define RA5_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define RA5_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define RA5_GetValue()              PORTAbits.RA5
#define RA5_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define RA5_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define RA5_SetAnalogMode()         do { ANCON0bits.PCFG4 = 1; } while(0)
#define RA5_SetDigitalMode()        do { ANCON0bits.PCFG4 = 0; } while(0)

// get/set IR_SW aliases
#define IR_SW_TRIS                 TRISBbits.TRISB0
#define IR_SW_LAT                  LATBbits.LATB0
#define IR_SW_PORT                 PORTBbits.RB0
#define IR_SW_WPU                  WPUBbits.WPUB0
#define IR_SW_ANS                  ANCON1bits.PCFG12
#define IR_SW_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define IR_SW_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define IR_SW_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define IR_SW_GetValue()           PORTBbits.RB0
#define IR_SW_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define IR_SW_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define IR_SW_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define IR_SW_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define IR_SW_SetAnalogMode()      do { ANCON1bits.PCFG12 = 1; } while(0)
#define IR_SW_SetDigitalMode()     do { ANCON1bits.PCFG12 = 0; } while(0)

// get/set RX1_TX0 aliases
#define RX1_TX0_TRIS                 TRISBbits.TRISB1
#define RX1_TX0_LAT                  LATBbits.LATB1
#define RX1_TX0_PORT                 PORTBbits.RB1
#define RX1_TX0_WPU                  WPUBbits.WPUB1
#define RX1_TX0_ANS                  ANCON1bits.PCFG10
#define RX1_TX0_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define RX1_TX0_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define RX1_TX0_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RX1_TX0_GetValue()           PORTBbits.RB1
#define RX1_TX0_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define RX1_TX0_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define RX1_TX0_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define RX1_TX0_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define RX1_TX0_SetAnalogMode()      do { ANCON1bits.PCFG10 = 1; } while(0)
#define RX1_TX0_SetDigitalMode()     do { ANCON1bits.PCFG10 = 0; } while(0)

// get/set RB4 procedures
#define RB4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()              PORTBbits.RB4
#define RB4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()             do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()           do { WPUBbits.WPUB4 = 0; } while(0)

// get/set RB5 procedures
#define RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()              PORTBbits.RB5
#define RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()             do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()           do { WPUBbits.WPUB5 = 0; } while(0)

// get/set color_LED_R aliases
#define color_LED_R_TRIS                 TRISCbits.TRISC0
#define color_LED_R_LAT                  LATCbits.LATC0
#define color_LED_R_PORT                 PORTCbits.RC0
#define color_LED_R_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define color_LED_R_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define color_LED_R_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define color_LED_R_GetValue()           PORTCbits.RC0
#define color_LED_R_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define color_LED_R_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)

// get/set color_LED_B aliases
#define color_LED_B_TRIS                 TRISCbits.TRISC1
#define color_LED_B_LAT                  LATCbits.LATC1
#define color_LED_B_PORT                 PORTCbits.RC1
#define color_LED_B_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define color_LED_B_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define color_LED_B_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define color_LED_B_GetValue()           PORTCbits.RC1
#define color_LED_B_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define color_LED_B_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)

// get/set COM_LED aliases
#define COM_LED_TRIS                 TRISCbits.TRISC2
#define COM_LED_LAT                  LATCbits.LATC2
#define COM_LED_PORT                 PORTCbits.RC2
#define COM_LED_ANS                  ANCON1bits.PCFG11
#define COM_LED_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define COM_LED_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define COM_LED_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define COM_LED_GetValue()           PORTCbits.RC2
#define COM_LED_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define COM_LED_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define COM_LED_SetAnalogMode()      do { ANCON1bits.PCFG11 = 1; } while(0)
#define COM_LED_SetDigitalMode()     do { ANCON1bits.PCFG11 = 0; } while(0)

// get/set RC4 procedures
#define RC4_GetValue()              PORTCbits.RC4

// get/set RC5 procedures
#define RC5_GetValue()              PORTCbits.RC5

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


#endif // PIN_MANAGER_H
/**
 End of File
*/