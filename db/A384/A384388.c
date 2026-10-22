/*
 * A384388  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (5^11*s+293183133) mod 2^30 when
 *          started at s=1.
 * https://oeis.org/A384388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(n) = (48828125 * a(n-1) + 293183133) mod 2^30 for n >
 *            1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* constants expressed in the widest type that can hold them */
    const uint64_t MULT  = 48828125ULL;        /* 5^11  */
    const uint64_t ADD   = 293183133ULL;
    const uint64_t MASK  = (1ULL << 30) - 1;   /* 2^30 – 1 for modulo */

    seq_t s = (seq_t)1;                      /* a(1)   */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* compute (MULT * s + ADD) modulo 2^30
           (casting to uint64_t keeps the intermediate product within bounds) */
        uint64_t prod = ((uint64_t)MULT * (uint64_t)s) % (1ULL << 30);
        prod = (prod + ADD) & MASK;
        s = (seq_t)prod;
    }

    return s;
}
