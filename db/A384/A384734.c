/*
 * A384734  Consecutive states of the linear congruential pseudo-random number
 *          generator (513*s+29741096258473) mod 2^47 when started at s=1.
 * https://oeis.org/A384734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (513*a(n-1) + 29741096258473) mod 2^47, with a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using iterative recurrence and mask */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2^47 - 1 as a mask */
    const seq_t mask      = (seq_t)(((uint64_t)1 << 47) - 1);
    const seq_t inc       = (seq_t)29741096258473ULL;
    const seq_t mult      = (seq_t)513;

    seq_t s = 1; /* a(1) */

    for (idx_t i = 1; i < n; ++i)
        s = (s * mult + inc) & mask; /* modulo 2^47 */

    return s;
}
