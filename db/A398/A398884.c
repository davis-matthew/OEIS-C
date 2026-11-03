/*
 * A398884  Array read by ascending antidiagonals: A(n,k) = n*k^n + 1, with k
 *          >= 0.
 * https://oeis.org/A398884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = i*j^i + 1 where the pair (i,j) is obtained from the index
 *            n by reading the array A(i,j)=i*j^i+1 along ascending
 *            antidiagonals (i+j increasing, with i decreasing within each
 *            diagonal)
 * Range:     u8 n <= 7, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
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

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for calculations */
    seq_t t   = (seq_t)n;

    /* Find the diagonal d such that (d+1)(d+2)/2 > n */
    seq_t rad = floor_sqrt(t * (seq_t)8 + 1);   /* sqrt(1 + 8*t) */
    seq_t d   = (rad - 1) / 2;                   /* floor((sqrt-1)/2) */

    /* Count of elements before diagonal d */
    seq_t prev = d * (d + 1) / 2;                /* C(d-1) */

    /* Position within the diagonal */
    seq_t k    = t - prev;                       /* 0 <= k <= d */

    /* Pair (i,j) on the diagonal: i decreases, j increases */
    seq_t i    = d - k;
    seq_t j    = k;

    /* Compute i * j^i + 1 */
    seq_t pow  = ipow(j, i);                  /* j^i */
    return i * pow + 1;
}
