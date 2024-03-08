#ifndef __RC522_FUNCTION_H
#define	__RC522_FUNCTION_H


#include "main.h"


#define          macDummy_Data              0x00


void             PcdReset                   ( void );                       //��λ
void             M500PcdConfigISOType       ( uint8_t type );                    //������ʽ
char             PcdRequest                 ( uint8_t req_code, uint8_t * pTagType ); //Ѱ��
char             PcdAnticoll                ( uint8_t * pSnr);                   //������
void IC_test ( void );


#endif /* __RC522_FUNCTION_H */

