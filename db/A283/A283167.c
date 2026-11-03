/*
 * A283167  a(0) = 1; a(2*n) = 2*a(n), a(2*n+1) = sigma(a(n)).
 * https://oeis.org/A283167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(2*n)=2*a(n); a(2*n+1)=sigma(a(n))
 * Range:     u8 n <= 255, u16 n <= 8734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9828 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
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

/* Compute the sum of divisors of a seq_t number */
static seq_t sigma_seq(seq_t x)
{
    seq_t sum = (seq_t)0;
    seq_t sqrt = floor_sqrt(x);
    for (seq_t d = (seq_t)1; d <= sqrt; ++d) {
        if (x % d == (seq_t)0) {
            seq_t q = x / d;
            sum += d;
            if (q != d) sum += q;
        }
    }
    return sum;
}

/* External interface required by the build */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* recursive definition uses integer arithmetic only */
    if ((n & (idx_t)1) == (idx_t)0) {          /* even n: a(2*k) = 2*a(k) */
        seq_t half = a(n >> (idx_t)1);
        return half * (seq_t)2;
    } else {                                   /* odd n: a(2*k+1) = sigma(a(k)) */
        seq_t half = a(n >> (idx_t)1);
        return sigma_seq(half);
    }
}
