/*
 * A372847  Number of unit squares enclosed by a circle of radius n with an
 *          even number of rows and the maximum number of squares in each row.
 * https://oeis.org/A372847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Sum_{k=1..n-1} floor(2*sqrt(n^2 - k^2))
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 8758 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
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
    seq_t sum = 0;
    idx_t N = n;                 /* use idx_t for intermediate arithmetic */
    for (idx_t k = 1; k < n; ++k) {
        idx_t t = N * N - k * k;           /* n^2 - k^2 as idx_t to avoid overflow */
        seq_t t_seq = (seq_t)t;            /* cast to seq_t for sqrt */
        seq_t s = floor_sqrt(t_seq);       /* floor(sqrt(t)) */
        seq_t threshold = s * s + s + 1;   /* ceil(s^2 + s + 0.25) */
        seq_t val = (t >= threshold) ? (2 * s + 1) : (2 * s);
        sum += val;
    }
    return sum << 1;                  /* 2 * sum */
}
