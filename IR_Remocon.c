//リモコン用
//家電協、NEC

//リモコンについて
//https://qiita.com/minter1999/items/fbb8a391d80e21c11e76
//https://akizukidenshi.com/download/k4174_format.pdf

//MCCでtimer2とPWM38kHzを設定。
//SHARPのTVリモコン、家電協フォーマット(48bit＝32bit(カスタムコード)+16bit(リーダーコード)
//カスタムコードは、メーカのコード。リモコンが混同しないように、リーダーコードが実際に何のボタンを押したか


#include "mcc_generated_files/mcc.h"
#include <xc.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "IR_Remocon.h"



//NECと家電協フォーマットのみ対応
void IR_RemoteControl_Send(uint8_t *IR_Data,uint8_t size)
{
    uint8_t codebyte;
    if (size==4)    //NECフォーマット
    {
        TMR2_StopTimer();   //タイマーを止めるだけではHi/Loで終わるか不安定だったため
        RPOR2 = 0x00;       //RA5をECCPから汎用ポートに
        RA5_SetLow();       //出力をLoに指定。3ステップ踏むことで、確実にLoにする

        __delay_ms(50);         // 安定化待ち

        //リーダーコード送信
        RPOR2 = 0x0E;       //RA5を汎用ポートからECCPに
        TMR2_StartTimer();  //タイマースタート
        __delay_us(9000);
        TMR2_StopTimer();
        RPOR2 = 0x00;
        RA5_SetLow();
        __delay_us(4500);

         // カスタム/データコード送信
        for (int i=0;i<size;i++)
        {
            codebyte = IR_Data[i];
            for(int j=7;j>=0;j--)
            {
                RPOR2 = 0x0E;
                TMR2_StartTimer();
                __delay_us(560);
                TMR2_StopTimer();
                RPOR2 = 0x00;
                RA5_SetLow();
                //PWM OFFの時間でOn/Off区別
                if (codebyte & (0b00000001 << j))
                {
                     __delay_us(1690);       //Bit on
                }
                else 
                {
                     __delay_us(560);        //Bit Off
                }
            }
        }
        //ストップビット送信
        RPOR2 = 0x0E;
        TMR2_StartTimer();
        __delay_us(560);
        TMR2_StopTimer();
        RPOR2 = 0x00;
        RA5_SetLow();
        __delay_ms(5);
    }
    
    if (size==6)    //家電協フォーマット
    {
        TMR2_StopTimer();   //タイマーを止めるだけではHi/Loで終わるか不安定だったため
        RPOR2 = 0x00;       //RA5をECCPから汎用ポートに
        RA5_SetLow();       //出力をLoに指定。3ステップ踏むことで、確実にLoにする

        __delay_ms(50);         // 安定化待ち

        //リーダーコード送信
        RPOR2 = 0x0E;       //RA5を汎用ポートからECCPに
        TMR2_StartTimer();  //タイマースタート
        __delay_us(3200);
        TMR2_StopTimer();
        RPOR2 = 0x00;
        RA5_SetLow();
        __delay_us(1600);

         // カスタム/データコード送信
        for (int i=0;i<6;i++)
        {
            codebyte = IR_Data[i];
            for(int j=7;j>=0;j--)
            {
                RPOR2 = 0x0E;
                TMR2_StartTimer();
                __delay_us(400);
                TMR2_StopTimer();
                RPOR2 = 0x00;
                RA5_SetLow();
                //PWM OFFの時間でOn/Off区別
                if (codebyte & (0b00000001 << j))
                {
                     __delay_us(1200);       //Bit on
                }
                else 
                {
                     __delay_us(400);        //Bit Off
                }
            }
        }
        //ストップビット送信
        RPOR2 = 0x0E;
        TMR2_StartTimer();
        __delay_us(400);
        TMR2_StopTimer();
        RPOR2 = 0x00;
        RA5_SetLow();
        __delay_ms(5);
    }

}


void IR_RemoteControl_Receive(void)
{
    unsigned char   rcv_data[6];
    TMR1_StopTimer();
    
    for (;;) {
      if (RA1==0){
          // リーダーコードの開始待ち
          IR_Receive_LED_SetHigh();
          while (IR_Receive_GetValue());
          IR_Receive_LED_SetLow();
          
          //リーダーコードの長さ測定
          TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1開始
          while (!(IR_Receive_GetValue()));
          TMR1_StopTimer();
          
          // HIになるまでの時間（ONの長さ）でフォーマット決定
          if ((TMR1H >= 0x5D) && (TMR1H <= 0x75)) {
              // ONが8ms-10.0msをNECフォーマットと判断
              // 一旦LOWになるのを待つ
              while (IR_Receive_GetValue());
              // データコード32ビット分繰り返す
              for (int i = 0; i < 4; i++) {
                  rcv_data[i] = 0 ;
                  for ( int j=7;j>=0;j--) {
                      while (!(IR_Receive_GetValue()));
                      TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1開始
                      while (IR_Receive_GetValue());
                      // LOWになるまでの時間（OFFの長さ）で0/1決定（1ms以上：1、1ms未満：0）
                      if (TMR1H >= 0x0B) {
                          rcv_data[i] = rcv_data[i] | (0b00000001 << j);
                      }
                  }
              }
          }else if((TMR1H >= 0x1D) && (TMR1H <= 0x2D)){
              // ONが2.5ms-3.9msを家電協フォーマットと判断
              // 一旦LOWになるのを待つ
              while (IR_Receive_GetValue());
              // データコード48ビット分繰り返す
              for (int i = 0; i < 6; i++) {
                  rcv_data[i] = 0 ;
                  for ( int j=7;j>=0;j--) {
                      while (!(IR_Receive_GetValue()));
                      TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1開始
                      while (IR_Receive_GetValue());
                      // LOWになるまでの時間（OFFの長さ）で0/1決定（0.8ms以上：1、0.8ms未満：0）
                      if (TMR1H >= 0x09) {
                          rcv_data[i] = rcv_data[i] | (0b00000001 << j);
                      }
                  }
              }
              
          } else {
              //不明のフォーマットを受信の合図
              IR_Receive_LED_SetHigh();__delay_ms(500);IR_Receive_LED_SetLow();__delay_ms(500);IR_Receive_LED_SetHigh();
              //もう一度、最初から
              continue;
          }
          // ストップビットはスキップ
          while (!(IR_Receive_GetValue()));
          TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1開始
          while (IR_Receive_GetValue()) {
          // 次信号までのタイミングで1フレーム送信の終了を確認
            if (TMR1H >= 0x08) {    //2.25ms-0.56ms=1.69ms 2ms位無ければ
                // 正常 EEPROM 0-3番地にデータ書き込み
                for (int eppaddr=0; eppaddr < 4; eppaddr++) {           
                    EEPROM_WRITE(eppaddr, rcv_data[eppaddr]);
                    FLASH_WriteByte(eppaddr, rcv_data[eppaddr], 1);
                }
                IR_Receive_LED_SetLow();
                return;
            }
          }
          //時間内に次の信号を受信（データコードの続き？）した場合、エラー
          IR_Receive_LED_SetHigh();__delay_ms(500);IR_Receive_LED_SetLow();__delay_ms(500);IR_Receive_LED_SetHigh();
          //もう一度、最初から
          continue;
      }
    }
    unsigned char   IR_EepData[6];
    unsigned char   size;
    for (int eppaddr=0; eppaddr < 6; eppaddr++) {
        IR_EepData[eppaddr] = FLASH_ReadByte(eppaddr);
    }
    while(1)
    {   
        if(IR_EepData[6]==0x00)
        {
            size=4;
            IR_RemoteControl_Send(IR_EepData,size);
        }
        else
        {
            size=6;
            IR_RemoteControl_Send(IR_EepData,size);
        }
        
    }
}

void IR_RemoteControl_Receive2(void)
{
    unsigned char   rcv_data[6];
    TMR1_StopTimer();
    
    for (;;) {
        
      if (RA1==0){
          printf("Receive\r\n");
//          __delay_ms(1000);
          // リーダーコードの開始待ち
          IR_Receive_LED_SetHigh();
          while (IR_Receive_GetValue());
          IR_Receive_LED_SetLow();
          
          //リーダーコードの長さ測定
          TMR1L=0;TMR1H=0;TMR1_StartTimer();     //Timer1開始
          while (!(IR_Receive_GetValue()));
          TMR1_StopTimer();
          
          // HIになるまでの時間（ONの長さ）でフォーマット決定
          if ((TMR1H >= 0x5D) && (TMR1H <= 0x75)) {
              printf("NEC\r\n");
              // ONが8ms-10.0msをNECフォーマットと判断
              // 一旦LOWになるのを待つ
              while (IR_Receive_GetValue());
              // データコード32ビット分繰り返す
              for (int i = 0; i < 4; i++) {
                  rcv_data[i] = 0 ;
                  for ( int j=7;j>=0;j--) {
                      while (!(IR_Receive_GetValue()));
                      TMR1L=0;TMR1H=0;TMR1_StartTimer();     //Timer1開始
                      while (IR_Receive_GetValue());
                      // LOWになるまでの時間（OFFの長さ）で0/1決定（1ms以上：1、1ms未満：0）
                      if (TMR1H >= 0x0B) {
                          rcv_data[i] = rcv_data[i] | (0b00000001 << j);
                      }
                  }
              }
          }else if((TMR1H >= 0x1D) && (TMR1H <= 0x2D)){
              printf("kaden\r\n");
              // ONが2.5ms-3.9msを家電協フォーマットと判断
              // 一旦LOWになるのを待つ
              while (IR_Receive_GetValue());
              // データコード48ビット分繰り返す
              for (int i = 0; i < 6; i++) {
                  rcv_data[i] = 0 ;
                  for ( int j=7;j>=0;j--) {
                      while (!(IR_Receive_GetValue()));
                      TMR1L=0;TMR1H=0;TMR1_StartTimer();     //Timer1開始
                      while (IR_Receive_GetValue());
                      // LOWになるまでの時間（OFFの長さ）で0/1決定（0.8ms以上：1、0.8ms未満：0）
                      if (TMR1H >= 0x09) {
                          rcv_data[i] = rcv_data[i] | (0b00000001 << j);
                      }
                  }
              }
              
          } else {
              printf("hatena\r\n");
              printf("H:%x L:%x\r\n",TMR1H,TMR1L);
              //不明のフォーマットを受信の合図
              IR_Receive_LED_SetHigh();__delay_ms(500);IR_Receive_LED_SetLow();__delay_ms(500);IR_Receive_LED_SetHigh();
              //もう一度、最初から
              continue;
          }
          // ストップビットはスキップ
          while (!(IR_Receive_GetValue()));
          TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1開始
          while (IR_Receive_GetValue()) {
          // 次信号までのタイミングで1フレーム送信の終了を確認
            if (TMR1H >= 0x13) {    //2.25ms-0.56ms=1.69ms 2ms位無ければ
                printf("0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\r\n",rcv_data[0],rcv_data[1],rcv_data[2],rcv_data[3],rcv_data[4],rcv_data[5]);
                return;
            }
          }
          //時間内に次の信号を受信（データコードの続き？）した場合、エラー
          IR_Receive_LED_SetHigh();__delay_ms(500);IR_Receive_LED_SetLow();__delay_ms(500);IR_Receive_LED_SetHigh();
          //もう一度、最初から
          continue;
      }
    }

    
}