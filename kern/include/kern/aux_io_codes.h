/* Supplied by UNSW for assignment 1 of the OS course. */
/* Not for broader distribution or re-use. */

#ifndef _KERN_AUX_IO_CODES_H_
#define _KERN_AUX_IO_CODES_H_


/* Protocol numbers for the aux-I/O system,
 * shared between kernel and user code.
 */


enum aux_io_subsystem {
	ROBOT_CORE = 6,
	ROBOT_SYNC_SYSTEM = 7,
	ROBOT_EMERGENCY_STOP = 8,
	ROBOT_LEFT_LEG = 13,
	ROBOT_RIGHT_LEG = 14,
	ROBOT_LEFT_ARM = 17,
	ROBOT_RIGHT_ARM = 18,
	ROBOT_SENSORS = 22,
	ROBOT_HEAD = 26,
	TEST_SYSTEM = 255,

	ROBOT_MAX_NUM_SUBSYSTEMS = 10,
};

enum aux_io_query {
	SYSTEM_STATUS = 1,
	SYSTEM_ACTION_RESULT = 2,
	READY_SUBSYSTEM = 3,
	SYSTEM_POSITION_X = 5,
	SYSTEM_POSITION_Y = 6,
	SYSTEM_POSITION_Z = 7,
	CIRCUIT_FREQUENCY = 17,
	CIRCUIT_CAPACITANCE = 18,
	BATTERY_LEVEL = 19,
	CIRCUIT_READY = 20,
	LIGHT_LEVEL = 23,
	SOUND_LEVEL = 24,
	TEMPERATURE = 25,
	SYSTEM_COVERING = 30,
};

enum aux_io_command {
	START_SEQUENCE = 1,
	END_SEQUENCE = 2,

	SYSTEM_RAISE = 5,
	SYSTEM_TWIST = 6,
	SYSTEM_TURN_AROUND = 7,
	ALL_SYSTEMS_GO = 42,
	SYSTEMS_PAUSE = 43,
	SYSTEM_SET_FREQ = 52,

	SYSTEM_SET_SYNC_MODE = 127,
};

enum aux_io_sequences {

	WAVE_SEQUENCE_1 = 5,
	WAVE_SEQUENCE_2 = 6,
	WAVE_SEQUENCE_3 = 7,

	QUERY_SEQUENCE = 31,
	ASYNC_NOTES = 63,
};


#endif

