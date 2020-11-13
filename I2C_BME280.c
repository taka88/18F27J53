#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "skI2Clib.h"
#include "skS11059-02DT.h"
#include "skI2CLCDlib.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "I2C_BME280.h"


unsigned long int temp_raw,hum_raw,pres_raw;
signed long int t_fine;

uint16_t dig_T1;
 int16_t dig_T2;
 int16_t dig_T3;
uint16_t dig_P1;
 int16_t dig_P2;
 int16_t dig_P3;
 int16_t dig_P4;
 int16_t dig_P5;
 int16_t dig_P6;
 int16_t dig_P7;
 int16_t dig_P8;
 int16_t dig_P9;
 int8_t  dig_H1;
 int16_t dig_H2;
 int8_t  dig_H3;
 int16_t dig_H4;
 int16_t dig_H5;
 int8_t  dig_H6;

void BME280_setup()
{
    uint8_t osrs_t = 1;             //Temperature oversampling x 1
    uint8_t osrs_p = 1;             //Pressure oversampling x 1
    uint8_t osrs_h = 1;             //Humidity oversampling x 1
    uint8_t mode = 3;               //Normal mode
    uint8_t t_sb = 5;               //Tstandby 1000ms
    uint8_t filter = 0;             //Filter off 
    uint8_t spi3w_en = 0;           //3-wire SPI Disable
    
    uint8_t ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
    uint8_t config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;
    uint8_t ctrl_hum_reg  = osrs_h;
    
//    Serial.begin(9600);
//    Wire.begin();
    
    BME280writeReg(0xF2,ctrl_hum_reg);
    BME280writeReg(0xF4,ctrl_meas_reg);
    BME280writeReg(0xF5,config_reg);
    BME280readTrim();                    //
}


void BME280_loop()
{
    double temp_act = 0.0, press_act = 0.0,hum_act=0.0, DI = 0.0;
    signed long int temp_cal;
    unsigned long int press_cal,hum_cal;
    char buf[16];
    
    BME280readData();
    temp_cal = BME280calibration_T(temp_raw);
    press_cal = BME280calibration_P(pres_raw);
    hum_cal = BME280calibration_H(hum_raw);
    temp_act = (double)temp_cal / 100.0;
    press_act = (double)press_cal / 100.0;
    hum_act = (double)hum_cal / 1024.0;
    //		不快指数演算　DI=0.81T+0.01H(0.99T-14.3)+46.3
	DI = 0.81*temp_act+0.01*hum_act*(0.99*temp_act-14.3)+46.3;
//    LCD_Puts("TEMP : ");
//    printf(LCD_Puts,"%5.1f ﾟC", temp_act);
    sprintf(buf,"%5.1fﾟC %5.1f%%", temp_act, hum_act);
    printf("%5.1f ﾟC %5.1f %%\r\n", temp_act, hum_act);
    LCD_SetCursor(0,0); //2列目へ移動
    LCD_Puts(buf);
//    sprintf(buf,"%6.1f hPa",press_act);
    sprintf(buf,"%6.1f hPa %5.1f",press_act,DI);
    printf("%6.1f hPa %5.1f\r\n",press_act,DI);
    LCD_SetCursor(0,1); //2列目へ移動
    LCD_Puts(buf);
//    LCD_Puts(hum_act);
//    LCD_Puts(" %");    
    
    __delay_ms(1000);
}

/*補正用のデータをレジスタから入手する*/
void BME280readTrim()
{
    uint8_t data[32],i=0;                      // Fix 2014/04/06
    int  ans ;
    
    ans =I2C_Start(BME280_ADDRESS,RW_0);
    if (ans == 0) {
        I2C_Send(0x88);
    }
    
    ans =I2C_rStart(BME280_ADDRESS,RW_1);
    for(int8_t i=0; i<=22; i++){
      data[i] = I2C_Receive(ACK);
    }
    data[23] = I2C_Receive(NOACK);
    I2C_Stop(); 
    
    // レジスタアドレス　0xA1のみ1バイト
    ans =I2C_Start(BME280_ADDRESS,RW_0);
    if (ans == 0) {
        I2C_Send(0xA1);
    }
    
    ans =I2C_rStart(BME280_ADDRESS,RW_1);
    data[24] =  I2C_Receive(NOACK);                     // Add 2014/04/06
    I2C_Stop();
    
    ans =I2C_Start(BME280_ADDRESS,RW_0);
    if (ans == 0) {
        I2C_Send(0xE1);
    }
    
    ans =I2C_rStart(BME280_ADDRESS,RW_1);
    for(int8_t i=25; i<=30; i++){
      data[i] = I2C_Receive(ACK);
    }
    data[31] = I2C_Receive(NOACK);
    I2C_Stop();

    dig_T1 = (data[1] << 8) | data[0];
    dig_T2 = (data[3] << 8) | data[2];
    dig_T3 = (data[5] << 8) | data[4];
    
    dig_P1 = (data[7] << 8) | data[6];
    dig_P2 = (data[9] << 8) | data[8];
    dig_P3 = (data[11]<< 8) | data[10];
    dig_P4 = (data[13]<< 8) | data[12];
    dig_P5 = (data[15]<< 8) | data[14];
    dig_P6 = (data[17]<< 8) | data[16];
    dig_P7 = (data[19]<< 8) | data[18];
    dig_P8 = (data[21]<< 8) | data[20];
    dig_P9 = (data[23]<< 8) | data[22];
    
    dig_H1 = data[24];
    dig_H2 = (data[26]<< 8) | data[25];
    dig_H3 = data[27];
    dig_H4 = (data[28]<< 4) | (0x0F & data[29]);
    dig_H5 = (data[30] << 4) | ((data[29] >> 4) & 0x0F); // Fix 2014/04/06
    dig_H6 = data[31];                                   // Fix 2014/04/06
}
void BME280writeReg(uint8_t reg_address, uint8_t data)
{
    int  ans ;
    ans =I2C_Start(BME280_ADDRESS,RW_0);
    if (ans == 0) {
        I2C_Send(reg_address);
        I2C_Send(data);
    }
    I2C_Stop(); 
}

/*湿度、湿度、気圧の生のデータをレジスタから入手する*/
void BME280readData()
{
    int i = 0;
    uint32_t data[8];
    int  ans ;
    
    ans =I2C_Start(BME280_ADDRESS,RW_0);
    if (ans == 0) {
        I2C_Send(0xF7);
    }

    
    ans =I2C_rStart(BME280_ADDRESS,RW_1);
    for(int8_t i=0; i<=6; i++){
      data[i] = I2C_Receive(ACK);
    }
    data[7] = I2C_Receive(NOACK);
    I2C_Stop();
    
    pres_raw = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    temp_raw = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
    hum_raw  = (data[6] << 8) | data[7];
}

/*生のデータを２の補正用データによって実際のデータに直す*/
signed long int BME280calibration_T(signed long int adc_T)
{
    
    signed long int var1, var2, T;
    var1 = ((((adc_T >> 3) - ((signed long int)dig_T1<<1))) * ((signed long int)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((signed long int)dig_T1)) * ((adc_T>>4) - ((signed long int)dig_T1))) >> 12) * ((signed long int)dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T; 
}

unsigned long int BME280calibration_P(signed long int adc_P)
{
    signed long int var1, var2;
    unsigned long int P;
    var1 = (((signed long int)t_fine)>>1) - (signed long int)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11) * ((signed long int)dig_P6);
    var2 = var2 + ((var1*((signed long int)dig_P5))<<1);
    var2 = (var2>>2)+(((signed long int)dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2)*(var1>>2)) >> 13)) >>3) + ((((signed long int)dig_P2) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((signed long int)dig_P1))>>15);
    if (var1 == 0)
    {
        return 0;
    }    
    P = (((unsigned long int)(((signed long int)1048576)-adc_P)-(var2>>12)))*3125;
    if(P<0x80000000)
    {
       P = (P << 1) / ((unsigned long int) var1);   
    }
    else
    {
        P = (P / (unsigned long int)var1) * 2;    
    }
    var1 = (((signed long int)dig_P9) * ((signed long int)(((P>>3) * (P>>3))>>13)))>>12;
    var2 = (((signed long int)(P>>2)) * ((signed long int)dig_P8))>>13;
    P = (unsigned long int)((signed long int)P + ((var1 + var2 + dig_P7) >> 4));
    return P;
}

unsigned long int BME280calibration_H(signed long int adc_H)
{
    signed long int v_x1;
    
    v_x1 = (t_fine - ((signed long int)76800));
    v_x1 = (((((adc_H << 14) -(((signed long int)dig_H4) << 20) - (((signed long int)dig_H5) * v_x1)) + 
              ((signed long int)16384)) >> 15) * (((((((v_x1 * ((signed long int)dig_H6)) >> 10) * 
              (((v_x1 * ((signed long int)dig_H3)) >> 11) + ((signed long int) 32768))) >> 10) + (( signed long int)2097152)) * 
              ((signed long int) dig_H2) + 8192) >> 14));
   v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * ((signed long int)dig_H1)) >> 4));
   v_x1 = (v_x1 < 0 ? 0 : v_x1);
   v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
   return (unsigned long int)(v_x1 >> 12);   
}