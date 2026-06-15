/* Supplied by UNSW as part of the Operating Systems course. */

#include <types.h>
#include <thread.h>
#include <syscall.h>
#include <aux_io.h>
#include <aux_io_system.h>
#include <kern/errno.h>
#include <kern/aux_io_codes.h>
#include <synch.h>

void
aux_io_system_setup (void)
{
}

void
aux_io_async_notify (int subsystem)
{
	(void)subsystem;
	panic("Called aux_io_async_notify() with async mode disabled.\n");
}

/* Implement the aux-I/O syscalls here. */

