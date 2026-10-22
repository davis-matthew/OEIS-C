/*
 * A384082  Consecutive states of the linear congruential pseudo-random number
 *          generator (61*s+323) mod 500 when started at s=1.
 * https://oeis.org/A384082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n) = (61*a(n-1) + 323) mod 500, for n ≥ 2.  The sequence is periodic with
   period 500, so we precompute one full period on the first call. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t period[500];
    static bool init = false;

    if (!init) {
        period[0] = (seq_t)1;                      /* a(1) = 1 */
        for (idx_t i = 1; i < 500; ++i) {
            /* All arithmetic is performed in seq_t. */
            seq_t val = ((seq_t)61 * period[i-1] + (seq_t)323) % (seq_t)500;
            period[i] = val;
        }
        init = true;
    }

    /* Convert the (1‑based) index to a 0‑based index into the period. */
    idx_t idx = (n - (idx_t)1) % (idx_t)500;
    return period[idx];
}
