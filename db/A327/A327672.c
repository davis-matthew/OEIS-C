/*
 * A327672  a(n) = Sum_{k=0..n} ceiling(sqrt(k)).
 * https://oeis.org/A327672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + floor(sqrt(n)))*(6*n - floor(sqrt(n)) -
 *            2*floor(sqrt(n))^2)/6
 * Range:     u8 n <= 49, u16 n <= 2107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values.
   Uses binary search with 128‑bit intermediate to avoid overflow. */
static idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low  = 1;
    idx_t high = ((idx_t)1 << 32);          /* sqrt(UINT64_MAX) < 2^32   */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t s = floor_sqrt_u64(n);              /* floor(sqrt(n))           */
    seq_t one_plus_s = (seq_t)(s + 1);        /* 1 + floor(sqrt(n))       */
    seq_t six_n = (seq_t)6 * (seq_t)n;        /* 6 * n                    */
    seq_t s_sq = (seq_t)s * (seq_t)s;         /* floor(sqrt(n))^2         */
    seq_t t2 = six_n - (seq_t)s - ((seq_t)2 * s_sq);  /* 6n - s - 2s^2 */
    seq_t result = one_plus_s * t2 / (seq_t)6; /* final division by 6      */
    return result;
}
