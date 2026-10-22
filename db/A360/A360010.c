/*
 * A360010  First part of the n-th weakly decreasing triple of positive
 *          integers sorted lexicographically. Each n > 0 is repeated
 *          A000217(n) times.
 * https://oeis.org/A360010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that (m+2)(m+1)m/6 ≥ n (m is the first
 *            component of the n‑th weakly decreasing triple)
 * Range:     u8 n <= 9130, u16 n <= 9130, u32 n <= 9130, u64 n <= 9130,
 *            u128 n <= 9130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline __uint128_t binom3(idx_t m)
{
    /* compute binomial coefficient (m+2 choose 3) = (m+2)(m+1)m/6 safely. */
    __uint128_t a = m;
    __uint128_t b = m + 1;
    __uint128_t c = m + 2;
    a = a * b;
    a = a * c;
    a /= 6;
    return a;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1. */
    idx_t low = 0;
    idx_t high;

    /* Initial guess for high using floating point cube root of 6*n,
       then increase until it is certainly large enough. */
    long double approx = pow((long double)6.0 * (long double)n, 1.0L / 3.0L);
    high = (idx_t)(approx) + 2;
    if (high < 1) high = 1;               /* safety for tiny n */

    while (binom3(high) < n) {
        if (high > (UINT64_MAX >> 1)) {    /* avoid overflow of high */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;                         /* double until sufficient */
    }

    /* Binary search for the smallest m with binom3(m) ≥ n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (binom3(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
