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


/* We STRONGLY recommend that you don't edit this file during assignment 1. */

struct aux_io_device *aux_io_device = NULL;

void
register_aux_io_device (struct aux_io_device *dev) {
	if ((dev->device_id & 1023) != 42) {
		panic("register_aux_io_device: not a known device: %d.\n",
			dev->device_id);
	}

	aux_io_device = dev;
}

struct aux_io_device *
get_aux_io_device (void) {
	return aux_io_device;
}

