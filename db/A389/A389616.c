/*
 * A389616  Maximum number of independent permutations on {0,1,...,n-1}.
 * https://oeis.org/A389616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor(log2(m)) where m is the smallest prime factor of n.
 *            a(1) is defined to be 1. For prime n, m=n
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t _smallest_prime_factor_uint64(uint64_t n)
{
    if (n <= 1) return n;          /* only used for n>=2 in main routine */
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return d;
    }
    return n;                      /* n is prime */
}

static uint64_t _floor_log2_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */
    uint64_t n_u = (uint64_t)n;
    uint64_t m = _smallest_prime_factor_uint64(n_u);
    uint64_t val = _floor_log2_uint64(m);
    return (seq_t)val;
}
