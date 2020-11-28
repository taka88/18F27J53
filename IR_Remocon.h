/* 
 * File:   IR_Remocon.h
 * Author: yukat
 *
 * Created on November 8, 2020, 12:34 PM
 */

#ifndef IR_REMOCON_H

void IR_RemoteControl_Send(uint8_t *IR_Data,uint8_t size);
void IR_RemoteControl_Receive(void);
void IR_RemoteControl_Receive2(void);

#define	IR_REMOCON_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* IR_REMOCON_H */

