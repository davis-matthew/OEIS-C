/*
 * A261951  Start with a single equilateral triangle for n=0; for the odd n-th
 *          generation add a triangle at each expandable vertex of the
 *          triangles of the (n-1)-th generation (this is the "vertex to
 *          vertex" version); for the even n-th generation use the "vertex to
 *          side" version; a(n) is the number of triangl...
 * https://oeis.org/A261951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261951:  a(0)=1, a(1)=3, a(2)=9, a(3)=12, a(4)=24,
   a(5)=24, a(6)=39, a(7)=27, a(8)=54, a(9)=33, a(10)=69
   Recurrence (for n>10):  a(n)=a(n-2)+a(n-8)-a(n-10)      */
/* This implementation computes a(n) in O(n) time, using a ring buffer
   of the last 11 computed values. No dynamic memory is allocated,
   so it works for any seq_t, including arbitrary‑precision types. */

#pragma once
#include <stdint.h>  /* for uintptr_t if needed, otherwise provided elsewhere */

seq_t a(idx_t n)
{
    /* base values for n = 0 .. 10 */
    static const seq_t base[11] = {
        1ULL, 3ULL, 9ULL, 12ULL, 24ULL,
        24ULL, 39ULL, 27ULL, 54ULL, 33ULL, 69ULL
    };

    if (n < 11)
        return base[n];

    /* ring buffer of the last 11 computed terms */
    seq_t ring[11];
    /* initialise the buffer with the base terms */
    for (idx_t i = 0; i < 11; ++i)
        ring[i] = base[i];

    /* compute subsequent terms up to n */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t t2  = ring[(i - 2) % 11];
        seq_t t8  = ring[(i - 8) % 11];
        seq_t t10 = ring[(i - 10) % 11];
        ring[i % 11] = t2 + t8 - t10;
    }

    return ring[n % 11];
}
