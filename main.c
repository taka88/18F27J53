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

//-------- LCD系--------------------------------------
#define I2CLCD_AQM1602 0x3e    // アドレス (7ビット表記)
#define LCD_SETCGRAMADDR 0x40
#define LCD_CLEARDISPLAY 0x01

//-------- カラーセンサ系 --------------------------------------
#define I2C_ColorSensor_S11059 0x2A
uint16_t c[4]={};


//-------- USB系 --------------------------------------
#include "mcc_generated_files/usb/usb.h"
static uint8_t readBuffer[64];
static uint8_t writeBuffer[64];


//-------- 赤外線リモコン用 -------------------
//家電協フォーマット、SHARPのTV
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
    
    //MCCで作成したコードだと設定がSSPEN(許可)、SSPM(モード設定))を初期では設定していない
    //使うときだけ、許可していると思われる。PIC外のI2Cモジュールのプロトコルに合わせるやり方が
    //不明なので、自作関数に任せる
    //初期設定をMCCに任せる。送受信等は自作関数に任せる。

    I2C_SSPCON1= 0x28;  //SSPEN:有効化、SSPM：I2Cマスタ
    
    LCD_Init_ST7032i(0,40,1,16);
    //シリアル通信でPCに表示。
    printf("1\r\n");



    __delay_ms(2000);
    BME280_setup();     //初期設定
    __delay_ms(2000);
    BME280readTrim();   //補正値データ読み出し
    __delay_ms(2000);
    char buf[16];
    LCD_Clear(); //BME280の都合で、クリアか何かしないとLCDが反応しない
    printf("10\r\n");
    //LCDで年月日表示できるようにフォントを登録
    uint8_t charmap[8];
    //年
    charmap[0] = 0b10000;
    charmap[1] = 0b11111;
    charmap[2] = 0b00100;
    charmap[3] = 0b11111;
    charmap[4] = 0b10100;
    charmap[5] = 0b11111;
    charmap[6] = 0b00100;
    charmap[7] = 0b00000;
    LCD_CreateChar(0, charmap);

    //月
    charmap[0] = 0b01111;
    charmap[1] = 0b01001;
    charmap[2] = 0b01111;
    charmap[3] = 0b01001;
    charmap[4] = 0b01111;
    charmap[5] = 0b01001;
    charmap[6] = 0b10001;
    charmap[7] = 0b00000;
    LCD_CreateChar(1, charmap);
    
    //日
    charmap[0] = 0b11111;
    charmap[1] = 0b10001;
    charmap[2] = 0b10001;
    charmap[3] = 0b11111;
    charmap[4] = 0b10001;
    charmap[5] = 0b10001;
    charmap[6] = 0b11111;
    charmap[7] = 0b00000;
    LCD_CreateChar(2, charmap);
    
    LCD_Clear();          // 全クリアしないと上で設定したキャラクタが反映されない
    
    __delay_ms(300);
    LCD_SetCursor(0,0); //(1列目, 1行目）へ移動
    LCD_Puts("2020");
    LCD_Putc(0x00);  //年
    LCD_Puts("10");
    LCD_Putc(0x01);  //月
    LCD_Puts("05");
    LCD_Putc(0x02);  //日
    __delay_ms(2000);
    printf("11\r\n");
    Color_init();    //カラーセンサの初期化+一回目読み込み。以降からは読み出しのみ
    while(1)
    {
        if(IR_SW_GetValue()==1)
        {
            //リモコンの配列をポインタで渡す
            uint8_t size;
            size = sizeof(IR_Data_Power)/sizeof(uint8_t);
            IR_RemoteControl_Send(IR_Data_Power,size);
        }
        
        Color_Read();
        //赤色でRC0を点灯させる
        if(c[0]>=0x0300){
            color_LED_R_SetHigh();
            color_LED_B_SetLow();
        }
        //青色でRC1を点灯させる
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
        
        //USBのVBUSの電圧で接続されたかを判断
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
            //USB通信で受信した内容に＋１をして送信する
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

        BME280_loop();  //温度、湿度、気圧を表示させる
        
        //USARTのシリアル通信で受信した結果でLEDを切り替え、結果を送信する
        if(EUSART1_is_rx_ready())
        {   
            uint8_t cmd;
            // UARTで受信していれば以下を実行する ---------------------
            cmd=EUSART1_Read();      // 受信したバイトを受け取る
            EUSART1_Write(cmd);      // 受信コードをエコーバック
            switch(cmd)
            {            // コマンドをチェック
                case 'H':           // 「H」なら　
                case 'h':{          // 「LED On」と送信し、LED On
                       COM_LED_SetHigh();
                       printf(" -> LED On!!  \r\n");             
                       break;
                   }
                case 'L':           // 「L」なら
                case 'l':{          // 「LED Off」と送信し、LED Off
                       COM_LED_SetLow();
                       printf(" -> LED Off!!  \r\n");   
                       break;
                   }
                default:{            // 以外は、「Fail」送信
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