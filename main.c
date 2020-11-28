/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC18F27J53
        Driver Version    :  2.00
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

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include <xc.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "skI2Clib.h"
#include "skS11059-02DT.h"
#include "skI2CLCDlib.h"
#include "I2C_BME280.h"
#include "My_I2C.h"
#include "IR_Remocon.h"


    char moji[] = "Hello, PIC World";
    char moji2[] = "Wak-tech";

//-------- LCD�n--------------------------------------
#define I2CLCD_AQM1602 0x3e    // �A�h���X (7�r�b�g�\�L)
#define LCD_SETCGRAMADDR 0x40
#define LCD_CLEARDISPLAY 0x01

//-------- �J���[�Z���T�n --------------------------------------
#define I2C_ColorSensor_S11059 0x2A
uint16_t c[4]={};


//-------- USB�n --------------------------------------
#include "mcc_generated_files/usb/usb.h"
static uint8_t readBuffer[64];
static uint8_t writeBuffer[64];


//-------- �ԊO�������R���p -------------------
//�Ɠd���t�H�[�}�b�g�ASHARP��TV
const unsigned char IR_Data_Power[6] ={ 0x55, 0x5A, 0xF1, 0x48, 0x68, 0x8B };//Power
const unsigned char IR_Data_Ch1[6] ={ 0x55, 0x5A, 0xF1, 0x48, 0x80, 0xC9 };//1ch
const unsigned char IR_Data_Ch2[6] ={ 0x55, 0x5A, 0xF1, 0x48, 0x40, 0xC5 };//2ch
const unsigned char IR_Data_Ch3[6] ={ 0x55, 0x5A, 0xF1, 0x48, 0xC0, 0xCD };//3ch
const unsigned char IR_Data_Ch4[6] ={ 0x55, 0x5A, 0xF1, 0x48, 0x20, 0xC3 };//4ch
const unsigned char IR_Data_Ch5[6] ={ 0x55, 0x5A, 0xF1, 0x48, 0xA0, 0xCB };//5ch
    
    
/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    color_LED_R_SetHigh();
    color_LED_B_SetHigh();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    __delay_ms(500);
    
    //MCC�ō쐬�����R�[�h���Ɛݒ肪SSPEN(����)�ASSPM(���[�h�ݒ�))�������ł͐ݒ肵�Ă��Ȃ�
    //�g���Ƃ������A�����Ă���Ǝv����BPIC�O��I2C���W���[���̃v���g�R���ɍ��킹�������
    //�s���Ȃ̂ŁA����֐��ɔC����
    //�����ݒ��MCC�ɔC����B����M���͎���֐��ɔC����B

    I2C_SSPCON1= 0x28;  //SSPEN:�L�����ASSPM�FI2C�}�X�^
    
    LCD_Init_ST7032i(0,40,1,16);
    //�V���A���ʐM��PC�ɕ\���B
    printf("1\r\n");



    __delay_ms(2000);
    BME280_setup();     //�����ݒ�
    __delay_ms(2000);
    BME280readTrim();   //�␳�l�f�[�^�ǂݏo��
    __delay_ms(2000);
    char buf[16];
    LCD_Clear(); //BME280�̓s���ŁA�N���A���������Ȃ���LCD���������Ȃ�
    printf("10\r\n");
    //LCD�ŔN�����\���ł���悤�Ƀt�H���g��o�^
    uint8_t charmap[8];
    //�N
    charmap[0] = 0b10000;
    charmap[1] = 0b11111;
    charmap[2] = 0b00100;
    charmap[3] = 0b11111;
    charmap[4] = 0b10100;
    charmap[5] = 0b11111;
    charmap[6] = 0b00100;
    charmap[7] = 0b00000;
    LCD_CreateChar(0, charmap);

    //��
    charmap[0] = 0b01111;
    charmap[1] = 0b01001;
    charmap[2] = 0b01111;
    charmap[3] = 0b01001;
    charmap[4] = 0b01111;
    charmap[5] = 0b01001;
    charmap[6] = 0b10001;
    charmap[7] = 0b00000;
    LCD_CreateChar(1, charmap);
    
    //��
    charmap[0] = 0b11111;
    charmap[1] = 0b10001;
    charmap[2] = 0b10001;
    charmap[3] = 0b11111;
    charmap[4] = 0b10001;
    charmap[5] = 0b10001;
    charmap[6] = 0b11111;
    charmap[7] = 0b00000;
    LCD_CreateChar(2, charmap);
    
    LCD_Clear();          // �S�N���A���Ȃ��Ə�Őݒ肵���L�����N�^�����f����Ȃ�
    
    __delay_ms(300);
    LCD_SetCursor(0,0); //(1���, 1�s�ځj�ֈړ�
    LCD_Puts("2020");
    LCD_Putc(0x00);  //�N
    LCD_Puts("10");
    LCD_Putc(0x01);  //��
    LCD_Puts("05");
    LCD_Putc(0x02);  //��
    __delay_ms(2000);
    printf("11\r\n");
    Color_init();    //�J���[�Z���T�̏�����+���ړǂݍ��݁B�ȍ~����͓ǂݏo���̂�
    while(1)
    {
        if(IR_SW_GetValue()==1)
        {
            //�����R���̔z����|�C���^�œn��
            uint8_t size;
            size = sizeof(IR_Data_Power)/sizeof(uint8_t);
            IR_RemoteControl_Send(IR_Data_Power,size);
        }
        
        Color_Read();
        //�ԐF��RC0��_��������
        if(c[0]>=0x0300){
            color_LED_R_SetHigh();
            color_LED_B_SetLow();
        }
        //�F��RC1��_��������
        else if(c[2]>=0x0300){
            color_LED_R_SetLow();
            color_LED_B_SetHigh();
        }
        else{
            color_LED_R_SetLow();
            color_LED_B_SetLow();
        }
        LCD_Clear() ;
        __delay_ms(1000);
        
        //USB��VBUS�̓d���Őڑ����ꂽ���𔻒f
        if(USB_attach_GetValue()==0)
        {
            if( USBGetDeviceState() < CONFIGURED_STATE )
            {
                return;
            }

            if( USBIsDeviceSuspended()== true )
            {
                return;
            }
            //USB�ʐM�Ŏ�M�������e�Ɂ{�P�����đ��M����
            if( USBUSARTIsTxTrfReady() == true)
            {
                uint8_t i;
                uint8_t numBytesRead;

                numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

                for(i=0; i<numBytesRead; i++)
                {
                    switch(readBuffer[i])
                    {
                        /* echo line feeds and returns without modification. */
                        case 0x0A:
                        case 0x0D:
                            writeBuffer[i] = readBuffer[i];
                            break;

                        /* all other characters get +1 (e.g. 'a' -> 'b') */
                        default:
                            writeBuffer[i] = readBuffer[i] + 1;
                            break;
                    }
                }

                if(numBytesRead > 0)
                {
                    putUSBUSART(writeBuffer,numBytesRead);
                }

            }
            CDCTxService();
        }

        BME280_loop();  //���x�A���x�A�C����\��������
        
        //USART�̃V���A���ʐM�Ŏ�M�������ʂ�LED��؂�ւ��A���ʂ𑗐M����
        if(EUSART1_is_rx_ready())
        {   
            uint8_t cmd;
            // UART�Ŏ�M���Ă���Έȉ������s���� ---------------------
            cmd=EUSART1_Read();      // ��M�����o�C�g���󂯎��
            EUSART1_Write(cmd);      // ��M�R�[�h���G�R�[�o�b�N
            switch(cmd)
            {            // �R�}���h���`�F�b�N
                case 'H':           // �uH�v�Ȃ�@
                case 'h':{          // �uLED On�v�Ƒ��M���ALED On
                       COM_LED_SetHigh();
                       printf(" -> LED On!!  \r\n");             
                       break;
                   }
                case 'L':           // �uL�v�Ȃ�
                case 'l':{          // �uLED Off�v�Ƒ��M���ALED Off
                       COM_LED_SetLow();
                       printf(" -> LED Off!!  \r\n");   
                       break;
                   }
                default:{            // �ȊO�́A�uFail�v���M
                       printf(" -> Fail Command!! \r\n");            
                       break;
                   }
            }
        }
        
        IR_RemoteControl_Receive2();
        
    }
        
}

/**
 End of File
*/