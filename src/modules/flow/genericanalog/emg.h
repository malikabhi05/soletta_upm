#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "sol-flow/genericanalog.h"

#include "sol-aio.h"
#include "sol-flow-internal.h"
#include "sol-flow.h"
#include "sol-mainloop.h"
#include "sol-util-internal.h"
#include "sol-types.h"

#include <mraa/aio.h>

#define CALIBRATION_LIMIT		1100

/**
 * Grove EMG sensor struct
 */
struct _upm_groveemg {
	mraa_aio_context	aio;
	uint8_t				pin;
	int32_t				value;
	int32_t				threshold;
};

/**
 * Opaque pointer type to Grove EMG struct
 */
typedef struct _upm_groveemg* upm_groveemg;

/**
 * Allocates memory to a Grove EMG struct
 *
 * @param pin Analog pin to use
 *
 * @return upm_groveemg struct
 */
upm_groveemg upm_groveemg_init(int pin);

/**
 * GroveEMG destructor
 *
 * @param upm_groveemg dev struct to deallocate memory
 */
void upm_groveemg_close(upm_groveemg dev);

/**
 * Calibrates the Grove EMG sensor
 *
 * @param upm_groveemg dev struct to deallocate memory
 */
void upm_groveemg_calibrate(upm_groveemg dev);

/**
 * Measures muscle signals from the reader
 *
 * @param upm_groveemg dev struct to deallocate memory
 * @return Muscle signals from the reader
 */
int upm_groveemg_value(upm_groveemg dev);
