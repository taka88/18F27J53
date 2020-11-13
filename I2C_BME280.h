/* 
 * File:   I2C_BME280.h
 * Author: yukat
 *
 * Created on October 31, 2020, 3:05 PM
 */

#ifndef I2C_BME280_H
#define	I2C_BME280_H


#define BME280_ADDRESS 0x76

void BME280_setup();
void BME280_loop();
void BME280readTrim();
void BME280writeReg(uint8_t reg_address, uint8_t data);
void BME280readData();
signed long int BME280calibration_T(signed long int adc_T);
unsigned long int BME280calibration_P(signed long int adc_P);
unsigned long int BME280calibration_H(signed long int adc_H);

#endif	/* I2C_BME280_H */