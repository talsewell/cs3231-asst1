/*
 * Supplied by UNSW as part of the OS course assignment 1.
 *
 * Not intended for further distribution or re-use.
 */

/*
 * left_wave program
 *
 * Attempts to wave the robot's left hand.
 *
 * This program should This should run to completion
 * as provided, but more attention to the robot spec is needed for it to
 * complete the wave.
 */

#include <aux_io_calls.h>
#include <kern/aux_io_codes.h>

int
main(void)
{

	/* Tell the test observer we are attempting the "wave" protocol. */
	//aux_action(TEST_SYSTEM, START_SEQUENCE, WAVE_SEQUENCE_1);

	/* Raise the arm high. */
	//aux_action(ROBOT_LEFT_ARM, SYSTEM_RAISE, 150);
	/* Drop the arm slightly. */
	//aux_action(ROBOT_LEFT_ARM, SYSTEM_RAISE, 80);
	/* Raise it again. */
	//aux_action(ROBOT_LEFT_ARM, SYSTEM_RAISE, 130);

	/* That's a wave! Or is it? */
	//aux_action(TEST_SYSTEM, END_SEQUENCE, 0);



	return 0;
}
