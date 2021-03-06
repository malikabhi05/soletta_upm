#include "emg.h"

// THIS IS A POC DRIVER FOR SOLETTA
// THIS DRIVER WILL BE REPLACED BY A UPM C DRIVER ONCE IT IS READY
// THIS DRIVER USES SOLETTA SPECIFIC UTILITY FUNCTIONS
// BECAUSE THATS ALL WE HAVE AVAILABLE AS OF NOW
// ALSO THIS DRIVER WILL NOT HAVE TO SIT IN THE CURRENT FOLDER IT IS IN
// IT WILL BE ALREADY PRESENT ON THE BOARD OR ELSEWHERE AND THEN ALL THAT HAS TO BE DONE
// IS TO ADD THE PKGCONFIG PATH

/*
THIS FUNCTION WILL BE IMPLEMENTED LATER
static void
upm_ms_sleep(int msecs)
{
}
*/

upm_groveemg upm_groveemg_init(int pin){
	upm_groveemg dev = (upm_groveemg) malloc(sizeof(struct _upm_groveemg));
	if(dev == NULL){
		//pr_info(LOG_MODULE_MAIN, "UPM GROVE EMG: Unable to assign memory");
		SOL_WRN("UPM GROVE EMG: Unable to assign memory");
		return NULL;
	}
	dev->aio = mraa_aio_init(pin);
	if(dev->aio == NULL){
		//pr_info(LOG_MODULE_MAIN, "UPM GROVE EMG: Unable to init pin");
		SOL_WRN("UPM GROVE EMG: Unable to init pin");
		free(dev);
		return NULL;
	}
	return dev;
}

void upm_groveemg_close(upm_groveemg dev){
	mraa_aio_close(dev->aio);
	free(dev);
}

void upm_groveemg_calibrate(upm_groveemg dev){
	//upm_ms_sleep(1000);
	int32_t i = 0, val, sum=0, threshold;

	for(i=0; i<CALIBRATION_LIMIT; i++){
		val = mraa_aio_read(dev->aio);
		sum += val;
		//upm_ms_sleep(10);
	}
	threshold = (int) sum/CALIBRATION_LIMIT;
	dev->threshold = threshold;
	//pr_info(LOG_MODULE_MAIN, "UPM GROVE EMG: Threshold: %d", threshold);
	//SOL_LOG("UPM GROVE EMG: Threshold : %d", threshold);
}

int upm_groveemg_value(upm_groveemg dev){
	SOL_WRN("UPM GROVE EMG: inside the get value function");
	return mraa_aio_read(dev->aio);
}
