/*
 * Supplied by UNSW as part of the OS course assignment 1.
 *
 * Not intended for further distribution or re-use.
 */

/*
 * queries program
 *
 * Attempts to perform some general queries.
 *
 */

#include <aux_io_calls.h>
#include <kern/aux_io_codes.h>
#include <errno.h>
#include <stdlib.h>

static void
check_result(int i) {
	int err;

	if (i >= 0) {
		/* Non-negative results indicate no error. */
		return;
	}
	/* Fetch the error from the standard library. */
	err = errno;
	/* It would make sense to print the errror, but there's no
	 * user-level output available yet. */
	(void) err;
	exit (255);
}

int
main(void)
{
	int i = -1;

	/* Tell the test observer we are attempting the "query" protocol. */
	//aux_action(TEST_SYSTEM, START_SEQUENCE, QUERY_SEQUENCE);

	/* Check the general system status. */
	//i = aux_query(ROBOT_CORE, SYSTEM_STATUS);
	check_result(i);

	/* TODO:
		- check the fuel/battery level at the robot core.
		- check the light level at the robot sensor array.
		- check for coverings or obstructions of the robot head.
	*/

	//aux_action(ROBOT_CORE, ALL_SYSTEMS_GO, 1);

	/* That's a wave! Or is it? */
	//aux_action(TEST_SYSTEM, END_SEQUENCE, 0);

	return 0;
}
