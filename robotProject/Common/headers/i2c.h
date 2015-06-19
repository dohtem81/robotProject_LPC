#ifndef I2C_H_
#define I2C_H_

//definitions of ports
#define		I2C0		(0x1 << 7)
#define		I2C1		(0x1 << 19)
#define		I2C2		(0x1 << 26)

//definitions for PINSELx
#define		I2C0_SDA	(0x1 << 22)
#define		I2C0_SCL	(0x1 << 24)
#define		I2C1_SDA	(0x3)
#define		I2C1_SCL	(0x3 << 2)
#define		I2C2_SDA	(0x2 << 20)
#define		I2C2_SCL	(0x2 << 22)

//slave address
#define		I2C0_SLAVE	0x02
#define		I2C1_SLAVE	0x04
#define		I2C2_SLAVE	0x06

//conset bits
#define		I2CONSET_AA		0x04
#define		I2CONSET_SI		0x08
#define		I2CONSET_STO	0x10
#define		I2CONSET_STA	0x20
#define		I2CONSET_I2EN	0x40

//function declarations
void initI2C(unsigned int);
void writeI2C(unsigned char);
void writeI2CStart(void);
void writeI2CStop(void);
void waitForComplete(void);
int  I2CreadRegister(unsigned short, unsigned short);


#endif /*I2C_H_*/
