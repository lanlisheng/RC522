#ifndef __RC522_FUNCTION_H
#define __RC522_FUNCTION_H

#include "main.h"

// #define u8 	uint8_t
// #define u32 uint32_t

#define macDummy_Data 0x00

void PcdReset(void);                                     //��λ
void M500PcdConfigISOType(uint8_t type);                 //������ʽ
uint8_t PcdRequest(uint8_t req_code, uint8_t *pTagType); //Ѱ��
uint8_t PcdAnticoll(uint8_t *pSnr);                      //������

#endif /* __RC522_FUNCTION_H */
