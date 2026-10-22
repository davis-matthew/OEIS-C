/*
 * A386206  Triangle read by rows: T(n,k) = n^2 - k, with 0 <= k <= n.
 * https://oeis.org/A386206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) gives the nth entry of the triangle T(n,k) = n^2 - k read
 *            row‑wise. For row r the values are r^2, r^2-1, … , r^2-r
 * Range:     u8 n <= 135, u16 n <= 3320, u32 n <= 3320, u64 n <= 3320,
 *            u128 n <= 3320, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for a 128‑bit unsigned integer.
   Returns floor(sqrt(x)). The result always fits in idx_t (uint64_t). */
static inline idx_t isqrt_u128(__int128 x)
{
    idx_t r = 0;
    idx_t b = (idx_t)1ULL << 63;               /* start with the highest bit */
    while (b) {
        idx_t t   = r + b;
        __int128 sq = (__int128)t * t;
        if (sq <= x) {
            r = t;
        }
        b >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(8*n+1)-1)/2) – the row index. */
    __int128 temp = (__int128)8 * n + 1;
    idx_t sq     = isqrt_u128(temp);
    idx_t r      = (sq - 1) / 2;

    /* S_prev = number of terms before row r = r*(r+1)/2. */
    idx_t s_prev = (r * (r + 1)) / 2;

    /* Position within the row. */
    idx_t k = n - s_prev;

    /* Value: r^2 - k, computed in seq_t. */
    seq_t val = (seq_t)r * (seq_t)r - (seq_t)k;
    return val;
}
