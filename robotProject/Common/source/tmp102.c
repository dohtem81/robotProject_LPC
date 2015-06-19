#include "../headers/tmp102.h"
#include "../headers/i2c.h"
#include "../headers/lpc21xx.h"


long TMP102_Read(){
	long ret;

	ret = ((long)(I2CreadRegister(TMP_REG, TMP_WR)));

	return ret;
}
