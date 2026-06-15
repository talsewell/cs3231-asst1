/* Supplied by UNSW for assignment 1 of the OS course. */
/* Not for broader distribution or re-use. */

#include <types.h>
#include <aux_io.h>
#include <synch.h>
#include <lib.h>

/* Functions of the aux-I/O subsystem as used directly from the kernel and not
   via the syscall layer. */


/* The (simulated) interrupt subsystem will call this function when an asyc
 * event completes. This will only happen when the async subsystem is on
 * (explained in later sections of the problem spec).
 */
void aux_io_async_notify (int subsystem);


/* Set up the aux-I/O system. */
void aux_io_system_setup (void);

/* Set up function for the simulated robot. */
void setup_simulated_robot(void);




