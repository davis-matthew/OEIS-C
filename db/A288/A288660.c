/*
 * A288660  Numbers whose cubes have an even number of digits.
 * https://oeis.org/A288660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 66, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9663 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose cubes have an even number of digits.
   This function finds the nth positive integer m such that m³ has an even
   number of decimal digits by scanning ranges of m where m³ lies between
   10^(k-1) and 10^k-1 for even values of k. */
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* iterate over even digit lengths k = 2,4,6,… */
    for (int k = 2; ; k += 2)
    {
        /* Bounds for the cube: [10^(k-1), 10^k-1] */
        seq_t pow10_km1 = pow10(k - 1);          /* 10^(k-1)  */
        seq_t t = floor_cube_root(pow10_km1);     /* floor(cuberoot(10^(k-1))) */
        seq_t t3 = t * t * t;
        seq_t lower = (t3 == pow10_km1) ? t : t + 1; /* ceil(cuberoot(10^(k-1))) */

        seq_t pow10_k = pow10(k);                 /* 10^k */
        seq_t pow10_k_minus_1 = pow10_k - (seq_t)1; /* 10^k - 1 */
        seq_t upper = floor_cube_root(pow10_k_minus_1);

        /* Number of terms in this block */
        seq_t block_len_seq = upper - lower + 1;
        idx_t  block_len    = (idx_t)block_len_seq;   /* safe: block_len < 2^64 */

        if (n <= block_len)            /* desired term is inside this block */
            return lower + (seq_t)(n - 1);

        /* skip to the next block */
        n -= block_len;
    }
}
