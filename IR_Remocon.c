//�����R���p
//�Ɠd���ANEC

//�����R���ɂ���
//https://qiita.com/minter1999/items/fbb8a391d80e21c11e76
//https://akizukidenshi.com/download/k4174_format.pdf

//MCC��timer2��PWM38kHz��ݒ�B
//SHARP��TV�����R���A�Ɠd���t�H�[�}�b�g(48bit��32bit(�J�X�^���R�[�h)+16bit(���[�_�[�R�[�h)
//�J�X�^���R�[�h�́A���[�J�̃R�[�h�B�����R�����������Ȃ��悤�ɁA���[�_�[�R�[�h�����ۂɉ��̃{�^������������


#include "mcc_generated_files/mcc.h"
#include <xc.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "IR_Remocon.h"



//NEC�ƉƓd���t�H�[�}�b�g�̂ݑΉ�
void IR_RemoteControl_Send(uint8_t *IR_Data,uint8_t size)
{
    uint8_t codebyte;
    if (size==4)    //NEC�t�H�[�}�b�g
    {
        TMR2_StopTimer();   //�^�C�}�[���~�߂邾���ł�Hi/Lo�ŏI��邩�s���肾��������
        RPOR2 = 0x00;       //RA5��ECCP����ėp�|�[�g��
        RA5_SetLow();       //�o�͂�Lo�Ɏw��B3�X�e�b�v���ނ��ƂŁA�m����Lo�ɂ���

        __delay_ms(50);         // ���艻�҂�

        //���[�_�[�R�[�h���M
        RPOR2 = 0x0E;       //RA5��ėp�|�[�g����ECCP��
        TMR2_StartTimer();  //�^�C�}�[�X�^�[�g
        __delay_us(9000);
        TMR2_StopTimer();
        RPOR2 = 0x00;
        RA5_SetLow();
        __delay_us(4500);

         // �J�X�^��/�f�[�^�R�[�h���M
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
                //PWM OFF�̎��Ԃ�On/Off���
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
        //�X�g�b�v�r�b�g���M
        RPOR2 = 0x0E;
        TMR2_StartTimer();
        __delay_us(560);
        TMR2_StopTimer();
        RPOR2 = 0x00;
        RA5_SetLow();
        __delay_ms(5);
    }
    
    if (size==6)    //�Ɠd���t�H�[�}�b�g
    {
        TMR2_StopTimer();   //�^�C�}�[���~�߂邾���ł�Hi/Lo�ŏI��邩�s���肾��������
        RPOR2 = 0x00;       //RA5��ECCP����ėp�|�[�g��
        RA5_SetLow();       //�o�͂�Lo�Ɏw��B3�X�e�b�v���ނ��ƂŁA�m����Lo�ɂ���

        __delay_ms(50);         // ���艻�҂�

        //���[�_�[�R�[�h���M
        RPOR2 = 0x0E;       //RA5��ėp�|�[�g����ECCP��
        TMR2_StartTimer();  //�^�C�}�[�X�^�[�g
        __delay_us(3200);
        TMR2_StopTimer();
        RPOR2 = 0x00;
        RA5_SetLow();
        __delay_us(1600);

         // �J�X�^��/�f�[�^�R�[�h���M
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
                //PWM OFF�̎��Ԃ�On/Off���
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
        //�X�g�b�v�r�b�g���M
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
          // ���[�_�[�R�[�h�̊J�n�҂�
          IR_Receive_LED_SetHigh();
          while (IR_Receive_GetValue());
          IR_Receive_LED_SetLow();
          
          //���[�_�[�R�[�h�̒�������
          TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1�J�n
          while (!(IR_Receive_GetValue()));
          TMR1_StopTimer();
          
          // HI�ɂȂ�܂ł̎��ԁiON�̒����j�Ńt�H�[�}�b�g����
          if ((TMR1H >= 0x5D) && (TMR1H <= 0x75)) {
              // ON��8ms-10.0ms��NEC�t�H�[�}�b�g�Ɣ��f
              // ��ULOW�ɂȂ�̂�҂�
              while (IR_Receive_GetValue());
              // �f�[�^�R�[�h32�r�b�g���J��Ԃ�
              for (int i = 0; i < 4; i++) {
                  rcv_data[i] = 0 ;
                  for ( int j=7;j>=0;j--) {
                      while (!(IR_Receive_GetValue()));
                      TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1�J�n
                      while (IR_Receive_GetValue());
                      // LOW�ɂȂ�܂ł̎��ԁiOFF�̒����j��0/1����i1ms�ȏ�F1�A1ms�����F0�j
                      if (TMR1H >= 0x0B) {
                          rcv_data[i] = rcv_data[i] | (0b00000001 << j);
                      }
                  }
              }
          }else if((TMR1H >= 0x1D) && (TMR1H <= 0x2D)){
              // ON��2.5ms-3.9ms���Ɠd���t�H�[�}�b�g�Ɣ��f
              // ��ULOW�ɂȂ�̂�҂�
              while (IR_Receive_GetValue());
              // �f�[�^�R�[�h48�r�b�g���J��Ԃ�
              for (int i = 0; i < 6; i++) {
                  rcv_data[i] = 0 ;
                  for ( int j=7;j>=0;j--) {
                      while (!(IR_Receive_GetValue()));
                      TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1�J�n
                      while (IR_Receive_GetValue());
                      // LOW�ɂȂ�܂ł̎��ԁiOFF�̒����j��0/1����i0.8ms�ȏ�F1�A0.8ms�����F0�j
                      if (TMR1H >= 0x09) {
                          rcv_data[i] = rcv_data[i] | (0b00000001 << j);
                      }
                  }
              }
              
          } else {
              //�s���̃t�H�[�}�b�g����M�̍��}
              IR_Receive_LED_SetHigh();__delay_ms(500);IR_Receive_LED_SetLow();__delay_ms(500);IR_Receive_LED_SetHigh();
              //������x�A�ŏ�����
              continue;
          }
          // �X�g�b�v�r�b�g�̓X�L�b�v
          while (!(IR_Receive_GetValue()));
          TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1�J�n
          while (IR_Receive_GetValue()) {
          // ���M���܂ł̃^�C�~���O��1�t���[�����M�̏I�����m�F
            if (TMR1H >= 0x08) {    //2.25ms-0.56ms=1.69ms 2ms�ʖ������
                // ���� EEPROM 0-3�Ԓn�Ƀf�[�^��������
                for (int eppaddr=0; eppaddr < 4; eppaddr++) {           
                    EEPROM_WRITE(eppaddr, rcv_data[eppaddr]);
                    FLASH_WriteByte(eppaddr, rcv_data[eppaddr], 1);
                }
                IR_Receive_LED_SetLow();
                return;
            }
          }
          //���ԓ��Ɏ��̐M������M�i�f�[�^�R�[�h�̑����H�j�����ꍇ�A�G���[
          IR_Receive_LED_SetHigh();__delay_ms(500);IR_Receive_LED_SetLow();__delay_ms(500);IR_Receive_LED_SetHigh();
          //������x�A�ŏ�����
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
          // ���[�_�[�R�[�h�̊J�n�҂�
          IR_Receive_LED_SetHigh();
          while (IR_Receive_GetValue());
          IR_Receive_LED_SetLow();
          
          //���[�_�[�R�[�h�̒�������
          TMR1L=0;TMR1H=0;TMR1_StartTimer();     //Timer1�J�n
          while (!(IR_Receive_GetValue()));
          TMR1_StopTimer();
          
          // HI�ɂȂ�܂ł̎��ԁiON�̒����j�Ńt�H�[�}�b�g����
          if ((TMR1H >= 0x5D) && (TMR1H <= 0x75)) {
              printf("NEC\r\n");
              // ON��8ms-10.0ms��NEC�t�H�[�}�b�g�Ɣ��f
              // ��ULOW�ɂȂ�̂�҂�
              while (IR_Receive_GetValue());
              // �f�[�^�R�[�h32�r�b�g���J��Ԃ�
              for (int i = 0; i < 4; i++) {
                  rcv_data[i] = 0 ;
                  for ( int j=7;j>=0;j--) {
                      while (!(IR_Receive_GetValue()));
                      TMR1L=0;TMR1H=0;TMR1_StartTimer();     //Timer1�J�n
                      while (IR_Receive_GetValue());
                      // LOW�ɂȂ�܂ł̎��ԁiOFF�̒����j��0/1����i1ms�ȏ�F1�A1ms�����F0�j
                      if (TMR1H >= 0x0B) {
                          rcv_data[i] = rcv_data[i] | (0b00000001 << j);
                      }
                  }
              }
          }else if((TMR1H >= 0x1D) && (TMR1H <= 0x2D)){
              printf("kaden\r\n");
              // ON��2.5ms-3.9ms���Ɠd���t�H�[�}�b�g�Ɣ��f
              // ��ULOW�ɂȂ�̂�҂�
              while (IR_Receive_GetValue());
              // �f�[�^�R�[�h48�r�b�g���J��Ԃ�
              for (int i = 0; i < 6; i++) {
                  rcv_data[i] = 0 ;
                  for ( int j=7;j>=0;j--) {
                      while (!(IR_Receive_GetValue()));
                      TMR1L=0;TMR1H=0;TMR1_StartTimer();     //Timer1�J�n
                      while (IR_Receive_GetValue());
                      // LOW�ɂȂ�܂ł̎��ԁiOFF�̒����j��0/1����i0.8ms�ȏ�F1�A0.8ms�����F0�j
                      if (TMR1H >= 0x09) {
                          rcv_data[i] = rcv_data[i] | (0b00000001 << j);
                      }
                  }
              }
              
          } else {
              printf("hatena\r\n");
              printf("H:%x L:%x\r\n",TMR1H,TMR1L);
              //�s���̃t�H�[�}�b�g����M�̍��}
              IR_Receive_LED_SetHigh();__delay_ms(500);IR_Receive_LED_SetLow();__delay_ms(500);IR_Receive_LED_SetHigh();
              //������x�A�ŏ�����
              continue;
          }
          // �X�g�b�v�r�b�g�̓X�L�b�v
          while (!(IR_Receive_GetValue()));
          TMR1L=0;TMR1H=0;TMR1ON=1;     //Timer1�J�n
          while (IR_Receive_GetValue()) {
          // ���M���܂ł̃^�C�~���O��1�t���[�����M�̏I�����m�F
            if (TMR1H >= 0x13) {    //2.25ms-0.56ms=1.69ms 2ms�ʖ������
                printf("0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\r\n",rcv_data[0],rcv_data[1],rcv_data[2],rcv_data[3],rcv_data[4],rcv_data[5]);
                return;
            }
          }
          //���ԓ��Ɏ��̐M������M�i�f�[�^�R�[�h�̑����H�j�����ꍇ�A�G���[
          IR_Receive_LED_SetHigh();__delay_ms(500);IR_Receive_LED_SetLow();__delay_ms(500);IR_Receive_LED_SetHigh();
          //������x�A�ŏ�����
          continue;
      }
    }

    
}