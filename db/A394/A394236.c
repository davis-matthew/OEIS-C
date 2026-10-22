/*
 * A394236  Number of circular parking functions of length n avoiding the
 *          pattern 123.
 * https://oeis.org/A394236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*binomial(2n-1, n-1)/(n+1) - binomial(n+1, 2) +
 *            (∑_{i=1..n} 2^gcd(i,n))/2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for idx_t values (<= 64‑bit) */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Fast exponentiation of 2^exp returning a seq_t */
static seq_t pow2_idx(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp) {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

/* Binomial coefficient C(n,k) computed multiplicatively */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (n - k + i) first, then divide by i */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Term 1: 2 * C(2n-1, n-1) / (n+1) */
    seq_t term1 = binom_idx(2 * n - 1, n - 1);
    term1 = term1 * 2;
    term1 = term1 / (seq_t)(n + 1);

    /* Term 2: C(n+1, 2) */
    seq_t term2 = binom_idx(n + 1, 2);

    /* Term 3: (sum_{i=1..n} 2^gcd(i,n)) / 2 */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t g = gcd_idx(i, n);
        sum += pow2_idx(g);
    }
    seq_t term3 = sum / 2;

    return term1 - term2 + term3;
}
