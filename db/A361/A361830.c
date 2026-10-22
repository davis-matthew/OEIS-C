/*
 * A361830  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} binomial(2*j,j) *
 *          binomial(k*j,n-j).
 * https://oeis.org/A361830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m, k) where * T(n,k) = Σ_{j=0..n} binom(2*j, j) *
 *            binom(k*j, n-j) * s = floor( (√(8*n+1)-1)/2 ) * m = n -
 *            s*(s+1)/2 * k = s - m
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Find the greatest integer s such that s*(s+1)/2 <= n. */
static inline idx_t find_diagonal(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Grow high until it exceeds the target. */
    while (((unsigned __int128)high * (high + 1) / 2) <= n)
        high <<= 1;
    /* Binary search between low and high. */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Compute T(n,k) for 0 <= n,k.  The sum has at most n+1 terms. */
static inline seq_t T_value(idx_t n, idx_t k)
{
    seq_t total = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t t1 = binom((idx_t)(2 * j), j);
        seq_t t2 = binom((idx_t)(k * j), n - j);
        total += t1 * t2;
    }
    return total;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    idx_t s = find_diagonal(n);
    /* triangular number s*(s+1)/2 fits in 128 bits. */
    idx_t before = (idx_t)(((unsigned __int128)s * (s + 1)) / 2);
    idx_t m = n - before;
    idx_t k = s - m;
    return T_value(m, k);
}
