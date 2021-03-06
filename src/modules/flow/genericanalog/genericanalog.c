/*
 * This file is part of the Soletta Project
 *
 * Copyright (C) 2015 Intel Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* C file for generic analog sensor
 * contains function similar to what a regular UPM driver would have
 */


#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// This file will be generated when you build soletta
#include "sol-flow/genericanalog.h"

// aio is not really required here
#include "sol-aio.h"
#include "sol-flow-internal.h"
#include "sol-flow.h"
#include "sol-mainloop.h"
#include "sol-util-internal.h"
#include "sol-types.h"
// include the driver header
#include "emg.h"

struct GroveEMG;
/*
 * Struct for the generic analog sensor
 */
struct _upm_generic_analog {
	struct sol_flow_node *node;
	struct sol_timeout *timer;
	struct _upm_groveemg *sensor;
	int	pin;
	int	mask;
	int	ret_data;
};

/*
 * This function reads the value from the sensor code/IO layer
 * Can also read in packets if there are any. But since this is the entry point for a sensor
 * it would usually just read in the sensor value directly, infact if UPM is used, the driver will
 * take care of reading the value.
 */
static int 
genericanalog_open(struct sol_flow_node *node, void *data, const struct sol_flow_node_options *options){
	int pin;
	struct _upm_generic_analog *mdata = data;

	// The following struct is the options structure, which contains the fields defined in the options field
	// of the json file. This structure gets populated with the values specified in the flow based program.
	const struct sol_flow_node_type_genericanalog_options *opts = (const struct sol_flow_node_type_genericanalog_options *)options;
    	SOL_FLOW_NODE_OPTIONS_SUB_API_CHECK(opts, SOL_FLOW_NODE_TYPE_GENERICANALOG_OPTIONS_API_VERSION, -EINVAL);

	// fetching the pin number from the opts structure
	pin = opts->pin;
    	if (opts->mask <= 0) {
        	SOL_WRN("aio (%d): Invalid bit mask value=%" PRId32 ".", opts->pin, opts->mask);
        	return -EINVAL;
    	}
	// initialize the pin
	mdata->sensor = upm_groveemg_init(opts->pin);
	
	// checks if NULL is returned by the driver
	SOL_NULL_CHECK_MSG(mdata->sensor, -EINVAL, "aio (%d): Couldn't be open. Maybe you used an invalid 'pin'=%d?", pin, pin);
	mdata->pin = opts->pin;
	// the mask can also be fetched from the options
	mdata->mask = 12;
	// setting this here is important, because it'll be used later
	// will cause error if not set
	mdata->node = node;
	return 0;
}

/*
 * generic close function
 */
static void 
genericanalog_close(struct sol_flow_node *node, void *data){
	struct _upm_generic_analog *mdata = data;

    	SOL_NULL_CHECK(mdata);
	// close the connection
	upm_groveemg_close(mdata->sensor);
	//check and close the timer
    	if (mdata->timer)
        	sol_timeout_del(mdata->timer);
}

/* 
 * Generic analog read value function
 * this function will call the get value function from the analog IO
 * Upon completion it calls a callback which contains the returned AIO value
 */
static int 
genericanalog_tick(struct sol_flow_node *node, void *data, uint16_t port, uint16_t conn_id, const struct sol_flow_packet *packet){
	struct _upm_generic_analog *mdata = data;
	int ret_value;

	// If you do not set the val parameter in this structure, then that would cause an error
	// but then that is exactly what it is for
	struct sol_irange val = {
		.min = 0,
		.max = 4094,
    	};

	SOL_NULL_CHECK(data, true);

	// fetching the value from the driver
	ret_value = upm_groveemg_value(mdata->sensor);
	// setting the value in the struct
	val.val = ret_value;
	SOL_WRN("read value: %d", ret_value);
	mdata->ret_data = ret_value;
	// sending out the packet on the out port
	sol_flow_send_irange_packet(mdata->node, SOL_FLOW_NODE_TYPE_GENERICANALOG__OUT__OUT, &val);
	return 0;
}
#include "genericanalog-gen.c"
