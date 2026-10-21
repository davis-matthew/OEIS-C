/*
 * A256749  a(n) is the time expressed in seconds starting at 0:00:00 at which
 *          the second-hand of a classical three hands clock is passing over
 *          any of the two other hands considering the minute-hand turns of 6
 *          degrees in one minute and the hour-hand of 6 degrees in 12
 *          minutes.
 * https://oeis.org/A256749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A256749 – times in seconds when the second hand passes over
   either the minute hand or the hour hand (discrete jumps).  The list
   is periodic with period 43200 seconds and 1248 terms per period. */

#include <stdint.h>
#include <stdbool.h>

static seq_t table[1248] = {0};
static bool initialized = false;

/* Build the 1248‐element table for one 12‑hour cycle.
   The clock is modeled with:  second hand ticks each second,
   minute hand jumps 6° every minute,
   hour   hand jumps 6° every 12 minutes (=720 sec).               */
static void build_cycle(void)
{
    if (initialized) return;
    idx_t count = 0;
    for (idx_t t = 0; t < 43200 && count < 1248; ++t)
    {
        idx_t sec = t % 60;
        idx_t min = (t / 60) % 60;
        idx_t hr  = (t / 720) % 12;
        if (sec == min || sec == hr)
            table[count++] = (seq_t)t;
    }
    initialized = true;
}

/* Return the n‑th term of A256749.  The sequence repeats every
   1248 terms with a fixed offset of 43200 seconds. */
seq_t a(idx_t n)
{
    build_cycle();
    idx_t block = n / 1248;          /* number of full periods */
    idx_t idx   = n % 1248;          /* position inside one period */
    return table[idx] + (seq_t)block * (seq_t)43200;
}
