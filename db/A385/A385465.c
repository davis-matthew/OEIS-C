/*
 * A385465  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (4096*s + 150889) mod 714025 when
 *          started at 1.
 * https://oeis.org/A385465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4096*a(n-1) + 150889) mod 714025, with a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1 */
    seq_t val = 1;

    /* If n is 1, we are already done.
       For n > 1, iterate the recurrence. */
    for (idx_t i = 1; i < n; ++i)
    {
        /* Compute the next state using a 64‑bit intermediate
           to avoid overflow on small seq_t types. */
        unsigned long long temp = 4096ULL * (unsigned long long)val + 150889ULL;
        temp %= 714025ULL;
        val = (seq_t)temp;
    }

    return val;
}
