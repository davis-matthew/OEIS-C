/*
 * A257212  Least d>0 such that floor(n/d) - floor(n/(d+1)) <= 1.
 * https://oeis.org/A257212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least d>0 such that floor(n/d)-floor(n/(d+1)) ≤ 1.
 *            Observation: the answer is always ≤ ceil(sqrt(n)), so we search
 *            d from 1 up to ceil(sqrt(n))
 * Range:     u8 n <= 688, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned integer (floor). */
static inline idx_t int_isqrt_u64(idx_t x)
{
    if (x == 0)
        return 0;
    idx_t lo = 1, hi = 4294967295ULL;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid > x / mid)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return hi;
}

/* Compute the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* ceil(sqrt(n)) */
    idx_t s = int_isqrt_u64(n);
    idx_t ceil_sqrt = s;
    if ((__uint128_t)(s + 1) * (s + 1) <= n)
        ceil_sqrt = s + 1;

    for (idx_t d = 1; d <= ceil_sqrt; ++d) {
        seq_t q = ((seq_t)n) / d;
        seq_t r = ((seq_t)n) / (d + 1);
        if (q <= r + 1)          /* equivalently q - r ≤ 1 */
            return (seq_t)d;
    }

    /* Fallback – should not be reached. */
    return (seq_t)(ceil_sqrt + 1);
}
