/*
 * Supplied by UNSW as part of the OS course assignment 1.
 *
 * Not intended for further distribution or re-use.
 */

/*
 * async_scale program
 *
 * Attempts to prime each of the limb circuits to match frequencies with the
 * notes of the C major chord in turn, and do this asynchronously for the four
 * robot limb.s
 *
 */

#include <aux_io_calls.h>
#include <kern/aux_io_codes.h>
#include <errno.h>
#include <stdlib.h>


int
main(void)
{
	/* Tell the test observer we are attempting the "async notes" protocol. */
	//aux_action(TEST_SYSTEM, START_SEQUENCE, ASYNC_NOTES); */

	/* Set the left arm in async mode. Have we forgotten anything? */
	//aux_action(ROBOT_LEFT_ARM, SYSTEM_SET_SYNC_MODE, 0);

	/* Tune the left arm circuit frequency to 262Hz, roughly middle C. */
	//aux_action(ROBOT_LEFT_ARM, SYSTEM_SET_FREQ, 262);

	/* TODO:
		- wait on the circuit frequency change.
		- also set frequencies 330Hz and 392Hz, middle E and middle G.
		- drive the other limbs as well.
	*/


	/* When that's done, end the sequence. */
	//aux_action(TEST_SYSTEM, END_SEQUENCE, 0);

	return 0;
}


