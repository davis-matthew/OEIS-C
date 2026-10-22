/*
 * A309081  a(n) = n - floor(n/2^2) + floor(n/3^2) - floor(n/4^2) + ...
 * https://oeis.org/A309081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(sqrt(n))} (-1)^(k+1)*floor(n/k^2)
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309081 */

#include <stddef.h>
#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n == 0 is beyond the offset, but return 0 for safety */
    if (n == 0) return 0;

    idx_t lim = isqrt_u64(n);
    seq_t sumOdd = 0, sumEven = 0;

    for (idx_t k = 1; k <= lim; ++k) {
        seq_t denom = (seq_t)k * (seq_t)k;
        seq_t term = (seq_t)n / denom;
        if (k & 1)
            sumOdd += term;
        else
            sumEven += term;
    }

    return sumOdd - sumEven;
}
