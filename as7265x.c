#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "i2c.h"
#include "as7265x.h"


#define I2C_AS72XX_SLAVE_STATUS_REG 0x00
#define I2C_AS72XX_SLAVE_WRITE_REG 0x01
#define I2C_AS72XX_SLAVE_READ_REG 0x02
#define I2C_AS72XX_SLAVE_TX_VALID 0x02
#define I2C_AS72XX_SLAVE_RX_VALID 0x01

/**
 * Read a I2C (real) register from AS7265x
 */
int i2cm_read(int i2c_fd, int addr) {
	uint8_t result;
	i2c_register_read(i2c_fd, 0x49, addr, &result);
	return result;
} 

/**
 * Write a I2C (real) register to AS7265x.
 */
int i2cm_write(int i2c_fd, int addr, int value) {
	i2c_register_write(i2c_fd, 0x49, addr, value);
}

/**
 * Write to AS7265x virtual register
 */
void as7265x_vreg_write(int i2c_fd, uint8_t virtualReg, uint8_t d)
{
	volatile uint8_t status;
	while (1)
	{
		// Read slave I²C status to see if the write buffer is ready.
		status = i2cm_read(i2c_fd,I2C_AS72XX_SLAVE_STATUS_REG);
		if ((status & I2C_AS72XX_SLAVE_TX_VALID) == 0) 
			// No inbound TX pending at slave. Okay to write now.
			break ;
	}
	// Send the virtual register address (enabling bit 7 to indicate a write).
	i2cm_write(i2c_fd, I2C_AS72XX_SLAVE_WRITE_REG, (virtualReg | 0x80)) ;
	while (1)
	{
		// Read the slave I²C status to see if the write buffer is ready.
		status = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_STATUS_REG) ;
		if ((status & I2C_AS72XX_SLAVE_TX_VALID) == 0)
		// No inbound TX pending at slave. Okay to write data now.
		break ;
	}
	// Send the data to complete the operation.
	i2cm_write(i2c_fd,I2C_AS72XX_SLAVE_WRITE_REG, d) ;
}

/**
 * Read from AS7265x virtual register 
 */
uint8_t as7265x_vreg_read(int i2c_fd, uint8_t virtualReg)
{
	volatile uint8_t status, d;

	status = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_STATUS_REG);
	if ( (status & I2C_AS72XX_SLAVE_RX_VALID) != 0)  {
		// data to be read
		d = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_READ_REG);
	}

	// Wait for WRITE flag to clear
	while (1)
	{
		// Read slave I²C status to see if the read buffer is ready.
		status = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_STATUS_REG) ;
		if ((status & I2C_AS72XX_SLAVE_TX_VALID) == 0)
		// No inbound TX pending at slave. Okay to write now.
		break;
	}


	// Send the virtual register address (disabling bit 7 to indicate a read).
	i2cm_write(i2c_fd, I2C_AS72XX_SLAVE_WRITE_REG, virtualReg);


	while (1)
	{
		// Read the slave I²C status to see if our read data is available.
		status = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_STATUS_REG);
		if ((status & I2C_AS72XX_SLAVE_RX_VALID)!= 0)
		// Read data is ready.
		break;
		else fprintf(stderr,"[%x] ",status);
	}

	// Read the data to complete the operation.
	d = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_READ_REG) ;
	return d;
}

/**
 * Select device
 *
 * @param device 0=master; 1=first slave; 2=second slave
 */
int as7265x_select_device(int i2c_fd, uint8_t device) {
	as7265x_vreg_write(i2c_fd, AS7265X_DEV_SELECT_CONTROL, device);
}

void as7265x_set_bulb_current(int i2c_fd, uint8_t current, uint8_t device)
{
	as7265x_select_device(i2c_fd,device);

	current &= 0b11;

	uint8_t value = as7265x_vreg_read(i2c_fd,AS7265X_LED_CONFIG);
	value &= 0b11001111; //Clear ICL_DRV bits
	value |= (current << 4); //Set ICL_DRV bits with user's choice
	as7265x_vreg_write(i2c_fd,AS7265X_LED_CONFIG, value);
}



void usage () {
	fprintf (stderr,"as7265x register [value]\n");
	fprintf (stderr,"read or write a AS7265X virtual register.\n");
}

void main (int argc, char **argv) {

	int i2c_fd;
	
	if (argc < 2) {
		usage();
		exit(0);
	}

	i2c_fd = i2c_init("/dev/i2c-1");

	int reg = atoi(argv[1]);

	if (argc >= 3) {
		int val = atoi(argv[2]);
		fprintf (stderr,"writing register %x with %x\n",reg,val);
		as7265x_vreg_write(i2c_fd,reg,val);
	} else {
		fprintf (stderr,"reading as7265x virtual register 0x%x, value=0x%x\n", reg, as7265x_vreg_read(i2c_fd,reg));
	}

}
