/* Supplied by UNSW for assignment 1 of the OS course. */

#ifndef _AUX_IO_H_
#define _AUX_IO_H_

struct aux_io_device;

typedef struct aux_io_device * aux_io_dev_ptr;

/* Don't modify this interface file in your assignment! */

/* Access structures for aux-I/O devices, including callbacks for the query
 * and command operations, and the get-subsystem mechanism.
 */
struct aux_io_device {
	int device_id;

	/* Query function, returns 0 or error-number on error, saves result
	 * of query to provided retval slot. Results are non-negative. */
	int (* query) (aux_io_dev_ptr d, int query, unsigned int *retval);

	/* Command function. No output. Returns once the robot has completed
	 * the command, except when running in asynchronous mode. */
	int (* command) (aux_io_dev_ptr d, int command, unsigned int arg);

	/* Access a subsystem of this device. This may return NULL on request
	 * for an invalid subsystem. Device access structures are limited:
	 * calling the subsystem access function may invalidate pointers
	 * previously returned by this function. */
	aux_io_dev_ptr (* subsystem) (aux_io_dev_ptr d, int subsystem_id);

	/* Used by the specific instance to store data. */
	void *data;
};

extern aux_io_dev_ptr get_aux_io_device (void);

extern void register_aux_io_device (aux_io_dev_ptr);

#endif


