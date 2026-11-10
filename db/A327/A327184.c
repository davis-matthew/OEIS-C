/*
 * A327184  Positions of 0's in {A327183(n) : n > 0}.
 * https://oeis.org/A327184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th k >= 1 with A327183(k) = 0, where
 *            A327183(k) = floor((2k+1)e) - floor((k+1)e) - floor(k e); found
 *            by a linear scan
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9942 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A327184_u128;

/* e truncated to 30 decimals, scaled by 10^30 */
static A327184_u128 A327184_floor_e(uint64_t k)
{
    static const A327184_u128 E = ((A327184_u128)2718281828459045ULL * (A327184_u128)1000000000000000ULL)
                                  + (A327184_u128)235360287471352ULL;
    static const A327184_u128 S = (A327184_u128)1000000000000000ULL * (A327184_u128)1000000000000000ULL;
    return ((A327184_u128)k * E) / S;
}

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < target) {
        k++;
        A327184_u128 v = A327184_floor_e(2 * k + 1) - A327184_floor_e(k + 1) - A327184_floor_e(k);
        if (v == 0) count++;
    }
    return (seq_t)k;
}
