/*
 * A327185  Positions of 1's in {A327183(n) : n > 0}.
 * https://oeis.org/A327185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327183(k) = 1, where A327183(k) =
 *            floor((2k+1)e) - floor((k+1)e) - floor(k e); found by linear
 *            scan
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9761 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A327185_u128;

static uint64_t A327185_floor_mul(uint64_t k)
{
    const A327185_u128 E = (A327185_u128)2718281828459045ULL * (A327185_u128)1000000000000000ULL
                         + (A327185_u128)235360287471352ULL;   /* e * 10^30, truncated */
    const A327185_u128 S = (A327185_u128)1000000000000000ULL * (A327185_u128)1000000000000000ULL;
    return (uint64_t)(((A327185_u128)k * E) / S);
}

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < target) {
        k++;
        int64_t v = (int64_t)A327185_floor_mul(2 * k + 1)
                  - (int64_t)A327185_floor_mul(k + 1)
                  - (int64_t)A327185_floor_mul(k);
        if (v == 1) count++;
    }
    return (seq_t)k;
}
