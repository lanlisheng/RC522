#include "rc522_config.h"








void RC522_Init ( void )
{
	macRC522_Reset_Disable();
	
	macRC522_CS_Disable();
}



