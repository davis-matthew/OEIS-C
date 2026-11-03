/*
 * A371625  The x-coordinate of the point (x,y) where x + y = n, x is an
 *          integer, and x/y is as close as possible to phi (by absolute
 *          difference).
 * https://oeis.org/A371625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A371625
 *  a(n) is the integer x∈[0,n] minimizing |x/(n−x)−φ|,  φ=(1+√5)/2.
 *  The algorithm uses the analytic identities:
 *      φ/(φ+1) = (√5−1)/2
 *  and compares the two candidates floor(n·(√5−1)/2) and
 *      ceil(n·(√5−1)/2)  (i.e. the only two integers that can minimise
 *      the distance).  The comparison is done exactly by integer arithmetic
 *      with 128‑bit intermediates, avoiding floating point.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compare the distances for x and x+1.
 * Returns true if x is better (or equal) than x+1.
 */
static bool lower_better(seq_t x, seq_t n)
{
    /* t = n − x  (≥1 because x < n) */
    seq_t t = n - x;
    if (t <= 1) {
        /* Only one possible division (x = n−1), so x is the best. */
        return true;
    }

    /* All calculations use 128‑bit intermediates to avoid overflow. */
    __int128 T = (__int128)t;
    __int128 D = T * (T - 1);                         /* t(t−1) */
    __int128 N = -2 * T * T + (2 * (__int128)n + 2) * T - (__int128)n;
    __int128 M = N - D;                                /* N−D */
    if (M <= 0)
        return false;                                 /* x+1 is better */

    /* Compare M² with 5·D² (both non‑negative). */
    __int128 left  = M * M;
    __int128 right = (__int128)5 * D * D;
    return left >= right;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n≥1; return 0 for n=0 (unused). */
    if (n <= 1)
        return 0;

    /* prod = 5·n²  (fits comfortably in seq_t for the required n). */
    seq_t prod = (seq_t)5 * (seq_t)n * (seq_t)n;
    seq_t root = floor_sqrt(prod);                     /* floor(√(5·n²)) */

    /* x0 = floor( (root − n) / 2 ) = floor( n·(√5−1)/2 ) */
    seq_t x0 = (root > n) ? (root - n) / 2 : 0;
    if (x0 >= n)
        x0 = n - 1;                                    /* safety guard */

    /* x0+1 would be invalid if it equals n (division by zero). */
    if (x0 + 1 == n)
        return x0;

    /* Choose between x0 and x0+1 by exact comparison. */
    return lower_better(x0, (seq_t)n) ? x0 : x0 + 1;
}
