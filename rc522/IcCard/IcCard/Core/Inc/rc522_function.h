#ifndef __RC522_FUNCTION_H
#define	__RC522_FUNCTION_H


#include "main.h"


#define u8 	uint8_t
#define u32 uint32_t

#define          macDummy_Data              0x00


void             PcdReset                   ( void );                       //��λ
void             M500PcdConfigISOType       ( u8 type );                    //������ʽ
char             PcdRequest                 ( u8 req_code, u8 * pTagType ); //Ѱ��
char             PcdAnticoll                ( u8 * pSnr);                   //������


#endif /* __RC522_FUNCTION_H */

