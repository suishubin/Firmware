/****************************************************************************
 *
 *   Copyright (C) 2012-2013 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file barosim.h
 *
 * Shared defines for the ms5611 driver.
 */
#include "VirtDevObj.hpp"

#define ADDR_RESET_CMD			0x1E	/* write to this address to reset chip */
#define ADDR_CMD_CONVERT_D1		0x48	/* write to this address to start pressure conversion */
#define ADDR_CMD_CONVERT_D2		0x58	/* write to this address to start temperature conversion */
#define ADDR_DATA			0x00	/* address of 3 bytes / 32bit pressure data */
#define ADDR_PROM_SETUP			0xA0	/* address of 8x 2 bytes factory and calibration data */
#define ADDR_PROM_C1			0xA2	/* address of 6x 2 bytes calibration data */

/* interface ioctls */
#define IOCTL_RESET			2
#define IOCTL_MEASURE			3

namespace barosim
{

/**
 * Calibration PROM as reported by the device.
 */
#pragma pack(push,1)
struct prom_s {
	uint16_t factory_setup;
	uint16_t c1_pressure_sens;
	uint16_t c2_pressure_offset;
	uint16_t c3_temp_coeff_pres_sens;
	uint16_t c4_temp_coeff_pres_offset;
	uint16_t c5_reference_temp;
	uint16_t c6_temp_coeff_temp;
	uint16_t serial_and_crc;
};

/**
 * Grody hack for crc4()
 */
union prom_u {
	uint16_t c[8];
	prom_s s;
};
#pragma pack(pop)

extern bool crc4(uint16_t *n_prom);

} /* namespace */

using namespace DriverFramework;

class BAROSIM_DEV : public VirtDevObj
{
public:
	BAROSIM_DEV();
	virtual ~BAROSIM_DEV();

	virtual int	init();
	virtual ssize_t	devRead(void *data, size_t count);
	virtual int	devIOCTL(unsigned long operation, unsigned long arg);

	virtual int	transfer(const uint8_t *send, unsigned send_len,
				 uint8_t *recv, unsigned recv_len);

protected:
	virtual void _measure();

private:
	/**
	 * Send a reset command to the barometer simulator.
	 *
	 * This is required after any bus reset.
	 */
	int		_reset();

	/**
	 * Send a measure command to the barometer simulator.
	 *
	 * @param addr		Which address to use for the measure operation.
	 */
	int		_doMeasurement(unsigned addr);

};
