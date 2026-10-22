/*
 * A377071  a(n) = binomial(bigomega(n) + omega(n) - 1, omega(n) - 1), where
 *          bigomega = A001222 and omega = A001221.
 * https://oeis.org/A377071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(bigomega(n)+omega(n)-1, omega(n)-1) where
 *            bigomega(n) and omega(n) are the total and distinct prime factor
 *            counts of n, respectively
 * Range:     u8 n <= 9239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is fully self‑contained and does not rely on any
   external helper functions.  Only standard headers are used. */
#include <stdint.h>
#include <stddef.h>

/* Compute bigomega (total number of prime factors, counted with
   multiplicity) and omega (number of distinct prime factors) of n.  */
static void factor_counts(uint64_t n, int *bigomega, int *omega)
{
    int W = 0;                /* bigomega */
    int w = 0;                /* omega  */
    uint64_t m = n;

    /* handle factor 2 */
    if ((m & 1) == 0) {
        do { m >>= 1; ++W; } while ((m & 1) == 0);
        ++w;
    }

    /* try odd divisors up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int this_w = 0;
            do { m /= p; ++W; ++this_w; } while (m % p == 0);
            if (this_w > 0) ++w;
        }
    }

    /* if any prime factor > sqrt(m) remains */
    if (m > 1) { ++W; ++w; }

    *bigomega = W;
    *omega    = w;
}

/* Compute binomial coefficient (n choose k) for 0 <= k <= n.  The
   calculation is performed in seq_t to avoid intermediate overflow
   whenever that is possible.  The division is exact at each step,
   so an integer result is guaranteed. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;            /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* Public sequence function required by the framework. */
seq_t a(idx_t n)
{
    /* Special case n == 1: bigomega and omega are both 0, hence
       the binomial coefficient would be undefined.  The sequence
       definition gives a(1) = 1. */
    if (n == (idx_t)1) return (seq_t)1;

    int W;          /* bigomega(n) */
    int w;          /* omega(n) */
    factor_counts((uint64_t)n, &W, &w);

    /* Prime powers and the number 1 have ω(n) == 1 for n ≥ 2;
       their value is 1 according to the OEIS entry. */
    if (w <= 1) return (seq_t)1;

    idx_t numerator   = (idx_t)W + (idx_t)w - (idx_t)1;
    idx_t denominator = (idx_t)w - (idx_t)1;   /* ω(n) – 1 >= 1 */

    return binom_idx(numerator, denominator);
}
