/*
 * A384971  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (106*s + 1283) mod 6075 when
 *          started at 1.
 * https://oeis.org/A384971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6075, u64 n <= 6075,
 *            u128 n <= 6075, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384971:  a(1)=1, a(n) = (106*a(n-1)+1283) mod 6075, n>=2.  */
/* Periodic with period 6075. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t cache[6075];
    if (!init)
    {
        seq_t s = 1;
        for (idx_t i = 0; i < 6075; ++i)
        {
            cache[i] = s;
            seq_t next = (seq_t)106 * s + 1283;
            s = next % 6075;
        }
        init = true;
    }

    idx_t idx = (n - 1) % 6075;
    return cache[idx];
}
