/*
 * A398340  Triangle read by rows, where T(n,k), n>0, k<=2^(n-1), is the
 *          smallest prime factor of (2*k-1)*2^n+1.
 * https://oeis.org/A398340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the smallest prime factor of ((2*k-1) * 2^r + 1) where r
 *            = 1 + floor(log2(n)) is the row index, k = n - 2^(r-1) + 1 is
 *            the column index within that row
 * Range:     u8 n <= 20, u16 n <= 320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the smallest prime factor of a 64‑bit integer.
   For the numbers that arise in this sequence (< 2^32 for n <= 10000)
   a simple trial division is fast enough. */
static seq_t spf_uint64(uint64_t n)
{
    if ((n & 1ULL) == 0) return (seq_t)2;          /* even number */
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return (seq_t)d;
    }
    return (seq_t)n;                                /* n is prime */
}

seq_t a(idx_t n)
{
    /* Determine the row (r) and column (k) from the linear index n. */
    idx_t r = 64 - __builtin_clzll(n);          /* floor(log2(n)) + 1 */
    idx_t base = 1ULL << (r - 1);               /* 2^(r-1) */
    idx_t k = n - base + 1;                     /* 1‑based column index */

    /* Compute the number whose smallest prime factor we need. */
    uint64_t val = (((uint64_t)(2 * k - 1)) << r) + 1ULL;

    /* Find and return the smallest prime factor of val. */
    return spf_uint64(val);
}
