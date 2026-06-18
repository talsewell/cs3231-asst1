/* Supplied by UNSW for assignment 1 of the OS course. */
/* Not for broader distribution or re-use. */

#include <types.h>
#include <kern/aux_io_codes.h>
#include <aux_io.h>
#include <aux_io_system.h>
#include <lib.h>
#include <kern/errno.h>
#include <thread.h>
#include <synch.h>
#include <proc.h>


struct aux_io_device *simulated_robot;

aux_io_dev_ptr subsystems[4];
int subsystem_ids[4];
int subsystem_counter;

int test_seq_type = 0;
int test_seq_time_step = 0;
int test_seq_states[8];
int test_seq_acts[8];
int test_complete = 0;
int test_failed = 0;

static void
seq_finished (const char *s) {
	kprintf("Test sequence finished: %s!\n", s);
	test_complete = 1;
}

static int
check_seq_finished (void) {
	if (test_complete) {
		kprintf("Confirmed, test sequence is done.\n");
		kprintf("Test sequence took %d action steps.\n", test_seq_time_step);
		return 0;
	}
	else {
		kprintf("Failed attempt to end-sequence, thread shutdown.\n");
		thread_exit();
		return EINVAL;
	}
}

static void
report_key_query(int i, const char *k, const char *v) {
	int j;
	if (test_seq_states[i] != 0) {
		kprintf("Repeat query of %s?\n", k);
		return;
	}
	kprintf("Queried %s: %s.\n", k, v);
	test_seq_states[i] = 1;
	for (j = 0; j < 4; j ++) {
		if (! test_seq_states[j]) {
			return;
		}
	}
	kprintf("Checks done. Hit it!\n");
	seq_finished("Queries.");
}

static int 
query_seq_query (int subsystem, int query, unsigned int *retval) {
	char arr[20];
	const char *s;
	int i, len;

	if (subsystem == ROBOT_CORE && query == SYSTEM_STATUS) {
		report_key_query(0, "robot status", "ready");
		*retval = 1;
	}

	if (subsystem == ROBOT_CORE && query == BATTERY_LEVEL) {
		report_key_query(1, "tank", "half full");
		*retval = 50;
	}

	if (subsystem == ROBOT_SENSORS && query == LIGHT_LEVEL) {
		report_key_query(2, "world", "it's dark");
		*retval = 3;
	}

	if (subsystem == ROBOT_HEAD && query == SYSTEM_COVERING) {
		s = "zhdulqj#vxqjodvvhv";
		len = strlen(s);
		for (i = 0; i < len; i ++) {
			arr[i] = s[i] - 3;
		}
		arr[i] = 0;
		report_key_query(3, "us", arr);
		*retval = 5;
	}

	return 0;
}



/* Possible results of attempting some moves at a particular time. */
enum move_res {
	MOVE_ERROR = 1,
	MOVE_STALL = 2,
	MOVE_MOVE = 3,
};

/* Arms are fiddly, and are available to move about 5 times in 16,
   and actually move about 2 times in 16. */
static int
arm_move_res(int time, int part_num) {
	int i;
	i = time & 15;
	i ^= 5;
	i += part_num * 6;
	i = i & 15;
	if (i < 2) {
		return MOVE_MOVE;
	}
	else if (i < 5) {
		return MOVE_STALL;
	}
	else {
		return MOVE_ERROR;
	}
}

static int
wave_check (int part_num, const char *name, unsigned int arg) {
	int i, ok;

	(void)name; /* Parameter is needed only for debugging code. */

	if (arg >= 200) {
		/* Too high! */
		test_failed = 1;
		return EINVAL;
	}

	i = arm_move_res(test_seq_time_step, part_num);
	if (i == MOVE_ERROR) {
		test_seq_acts[part_num] = 1;
		test_failed = 1;
		return EINVAL;
	}
	else if (i == MOVE_STALL) {
		/* Stalls fail silently, the user has to query them. */
		test_seq_acts[part_num] = 0;
		return 0;
	}

	test_seq_acts[part_num] = 1;

	/* Does this match the wave sequence? */
	/* Sequence is 0->1 high, 1->2 medium, 2->3 high, 3 is finished. */
	i = test_seq_states[part_num];
	ok = 0;
	if ((i == 0 || i == 2) && arg > 100 && arg < 200) {
		ok = 1;
	}
	else if ((i == 1) && arg > 50 && arg < 100) {
		ok = 1;
	}

	if (ok) {
		test_seq_states[part_num] ++;
	}
	else {
		test_failed = 1;
	}
	return 0;
}

static int
wave_query (int part_num, int query, unsigned int *retval) {
	int i;

	switch (query) {
		case SYSTEM_STATUS:
			*retval = 1;
			break;
		case SYSTEM_ACTION_RESULT:
			*retval = test_seq_acts[part_num];
			break;
		case CIRCUIT_READY:
			i = arm_move_res(test_seq_time_step, part_num);
			*retval = (i == MOVE_ERROR) ? 0 : 1;
			break;
		default:
			*retval = 0;
			break;
	}
	return 0;
}



static int
left_hand_wave_check (int subsystem, int command, unsigned int arg) {
	int res;

	if (subsystem != ROBOT_LEFT_ARM) {
		return ENOPROTOOPT;
	}

	if (command != SYSTEM_RAISE) {
		return ENOPROTOOPT;
	}

	res = wave_check(0, "left", arg);
	if (res) {
		return res;
	}

	if (test_seq_states[0] == 3 && test_failed == 0) {
		seq_finished("Left arm wave!");
	}
	return 0;
}

static int
left_hand_wave_query (int subsystem, int query, unsigned int *retval) {
	if (subsystem != ROBOT_LEFT_ARM) {
		return ENOPROTOOPT;
	}

	return wave_query (0, query, retval);
}


static int
double_wave_check (int subsystem, int command, unsigned int arg) {
	int res;

	if (command != SYSTEM_RAISE) {
		return ENOPROTOOPT;
	}

	if (subsystem == ROBOT_LEFT_ARM) {
		res = wave_check(0, "left", arg);
	}
	else if (subsystem == ROBOT_RIGHT_ARM) {
		res = wave_check(1, "right", arg);
	}
	else {
		return ENOPROTOOPT;
	}

	if (res) {
		return res;
	}

	if (test_seq_states[0] == 3 && test_seq_states[1] == 3 && test_failed == 0) {
		seq_finished("Double wave!");
	}
	return 0;
}

static int
double_wave_query (int subsystem, int query, unsigned int *retval) {

	if (subsystem == ROBOT_LEFT_ARM) {
		return wave_query(0, query, retval);
	}
	else if (subsystem == ROBOT_RIGHT_ARM) {
		return wave_query(1, query, retval);
	}
	else {
		return ENOPROTOOPT;
	}
}



static int
init_seq (int subsystem, int command, unsigned int arg) {
	int i;

	/* At this point we have to be starting up a sequence. */
	if (! (subsystem == TEST_SYSTEM && command == START_SEQUENCE &&
			test_seq_type == 0)) {
		return EINVAL;
	}

	switch (arg) {
		case WAVE_SEQUENCE_1:
		case WAVE_SEQUENCE_2:
		case QUERY_SEQUENCE:
		case ASYNC_NOTES:
			test_seq_type = arg;
			break;

		default:
			return EINVAL;
	}

	test_seq_time_step = 0;
	for (i = 0; i < 8; i ++) {
		test_seq_states[i] = 0;
		test_seq_acts[i] = 0;
	}
	return 0;
}


static int
pause_step (int subsystem) {
	if (subsystem != ROBOT_CORE) {
		return EINVAL;
	}

	test_seq_time_step++;

	return 0;
}


struct lock *async_sys_lock;
struct cv *async_wait_cv;

int async_thread_running = 0;

int subsystems_to_reply[32];
int subsystems_in_async[32];

static void
async_notify_thread(void *unused_ptr, unsigned long unused_int) {
	int i;

	(void) unused_ptr;
	(void) unused_int;

	lock_acquire(async_sys_lock);
	if (async_thread_running != 1) {
		panic("async_notify_thread: async_thread_running: %d.\n",
			async_thread_running);
	}

	/* Initialise structures. */
	for (i = 0; i < 32; i ++) {
		subsystems_in_async[i] = 0;
		subsystems_to_reply[i] = 0;
	}
	/* Notify other threads that startup is complete. */
	async_thread_running = 2;
	cv_broadcast(async_wait_cv, async_sys_lock);

	while (1) {
		for (i = 0; i < 32 && (! subsystems_to_reply[i]); i ++) {
			/* Loop. */
		}

		if (i == 32) {
			/* Nothing to do. */
			cv_wait(async_wait_cv, async_sys_lock);
		}
		else {
			subsystems_to_reply[i] = 0;
			aux_io_async_notify(i);
		}
	}
}

static int
try_startup_async_notify_thread(void) {
	int result;

	lock_acquire(async_sys_lock);
	if (async_thread_running) {
		while (async_thread_running != 2) {
			cv_wait(async_wait_cv, async_sys_lock);
		}
		lock_release(async_sys_lock);
		return 0;
	}
	result = thread_fork("async_notify_thread", kproc,
		async_notify_thread, NULL, 0);

	async_thread_running = 1;
	/* Wait for the async thread to start up and initialise. */
	while (async_thread_running != 2) {
		cv_wait(async_wait_cv, async_sys_lock);
	}
	lock_release(async_sys_lock);
	return result;
}

static int
is_setup_async(void) {
	int i;

	lock_acquire(async_sys_lock);
	i = async_thread_running;
	lock_release(async_sys_lock);

	return i;
}

static int
set_sync_mode(int subsystem_id) {
	if (subsystem_id == ROBOT_SYNC_SYSTEM) {
		return try_startup_async_notify_thread();
	}
	if (! (is_setup_async())) {
		test_failed = 1;
		return EINVAL;
	}
	lock_acquire(async_sys_lock);
	subsystems_in_async[subsystem_id] = 1;
	lock_release(async_sys_lock);

	return 0;
}

static int
async_freq_check (int subsystem, int command, unsigned int arg) {
	int proto_ok;

	lock_acquire(async_sys_lock);
	proto_ok = subsystems_in_async[subsystem] &&
		(! subsystems_to_reply[subsystem]) &&
		test_seq_states[subsystem - 12] < 3 &&
		command == SYSTEM_SET_FREQ;

	/* Maybe we should check the arguments. */
	(void)arg;
	
	if (! proto_ok) {
		/* In principle it's OK to drive this system in synchronous
		 * mode, but that's unimplemented, just mark as a failure. */
		test_failed = 1;
		lock_release(async_sys_lock);
		return EINVAL;
	}

	subsystems_to_reply[subsystem] = 1;
	cv_broadcast(async_wait_cv, async_sys_lock);

	test_seq_states[subsystem - 12] ++;
	test_seq_states[0] ++;

	if (test_seq_states[0] == 12) {
		seq_finished("Async Frequency Scale!");
	}

	lock_release(async_sys_lock);
	return 0;
}

static int
command (aux_io_dev_ptr d, int command, unsigned int arg) {
	int res;
	int *id_ptr, subsystem_id;

	id_ptr = (int *)(d->data);
	if (id_ptr == NULL) {
		return ENODEV;
	}
	subsystem_id = *id_ptr;

	/* Global operations (available in any protocol). */
	switch (command) {
		case START_SEQUENCE:
			return init_seq(subsystem_id, command, arg);

		case END_SEQUENCE:
			return check_seq_finished();

		case SYSTEMS_PAUSE:
			return pause_step(subsystem_id);

		case SYSTEM_SET_SYNC_MODE:
			return set_sync_mode(subsystem_id);
	}

	switch (test_seq_type) {

		case WAVE_SEQUENCE_1:
			res = left_hand_wave_check(subsystem_id, command, arg);
			break;

		case WAVE_SEQUENCE_2:
			res = double_wave_check(subsystem_id, command, arg);
			break;

		case ASYNC_NOTES:
			res = async_freq_check(subsystem_id, command, arg);
			break;

		default:
			return EINVAL;
	}

	res = test_seq_time_step++;
	return res;
}

static int
query (aux_io_dev_ptr d, int query, unsigned int *retval) {

	int res;
	int *id_ptr, subsystem_id;

	id_ptr = (int *)(d->data);
	if (id_ptr == NULL) {
		return ENODEV;
	}
	subsystem_id = *id_ptr;

	switch (test_seq_type) {

		case WAVE_SEQUENCE_1:
			res = left_hand_wave_query(subsystem_id, query, retval);
			break;

		case WAVE_SEQUENCE_2:
			res = double_wave_query(subsystem_id, query, retval);
			break;

		case QUERY_SEQUENCE:
			res = query_seq_query(subsystem_id, query, retval);
			break;

		default:
			*retval = 0;
			return 0;
	}

	return res;
}

static aux_io_dev_ptr
get_subsystem (aux_io_dev_ptr d, int subsystem_id) {
	int i;
	int *id_ptr;

	if (d->data) {
		return NULL;
	}

	i = (subsystem_counter + 1) & 3;
	subsystem_counter = i;
	id_ptr = (int *)(subsystems[i]->data);
	*id_ptr = subsystem_id;
	return subsystems[i];
}

static aux_io_dev_ptr
cannot_get_subsystem (aux_io_dev_ptr d, int subsystem_id) {
	(void) d;
	(void) subsystem_id;
	return NULL;
}

static aux_io_dev_ptr
alloc_device (void) {
	struct aux_io_device *dev;

	dev = kmalloc(sizeof(struct aux_io_device));
	if (dev == NULL) {
		panic("Unable to allocate memory for simulated robot.\n");
	}

	dev->device_id = (5 << 12) + 42;
	dev->command = command;
	dev->query = query;
	dev->subsystem = get_subsystem;
	dev->data = NULL;

	return dev;
}

void
setup_simulated_robot (void) {

	int i;
 
	if (simulated_robot != NULL) {
		panic("Simulated robot already set up.\n");
	}

	simulated_robot = alloc_device();

	for (i = 0; i < 4; i ++) {
		subsystems[i] = alloc_device();
		subsystems[i]->subsystem = cannot_get_subsystem;
		subsystems[i]->data = (void *)(&(subsystem_ids[i]));
	}

	async_sys_lock = lock_create("async system lock");
	if (async_sys_lock == NULL) {
		panic("Could not allocate async system lock.\n");
	}
	async_wait_cv = cv_create("async system CV");
	if (async_wait_cv == NULL) {
		panic("Could not allocate async system CV.\n");
	}
	register_aux_io_device(simulated_robot);
}


