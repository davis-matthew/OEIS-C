/*
 * A384290  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (214013*s+10395331) mod 2^24 when
 *          started at s=1.
 * https://oeis.org/A384290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (214013 * a(n-1) + 10395331) mod 2^24, with a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lcg_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the seq_t type provided by the build environment. */
#include <stdint.h>
#include "../utils/lcg_next.h"

/* Return the nth term of the sequence.  The function works for any
   idx_t (uint64_t) and any seq_t width (8, 16, 32, 64 or 128 bits,
   or arbitrary precision).  Calls need not be in increasing order
   and the function recomputes the state in O(n) time, which is well
   below the 2‑second limit for n ≤ 10 000. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) is the first term listed. */
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t state = 1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        state = lcg_next(state);
    }
    return state;
}
