/*
 * A365247  G.f. satisfies A(x) = 1 + x*A(x)^2/(1 - x^3*A(x)^4).
 * https://oeis.org/A365247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n-2*k-1,k) *
 *            binomial(2*n-2*k+1,n-3*k) / (2*n-2*k+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition given in OEIS A365247. */
#include <stdint.h>

/* multiplicative binomial coefficient, safe for idx_t up to 64‑bit and seq_t arbitrary precision */
static seq_t binom_mul(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first, then divide – the result is always integer */
        res = res * (n - k + i) / i;
    }
    return res;
}

/* sequence A365247 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;              /* base case */
    seq_t sum = 0;
    idx_t lim = n / 3;
    for (idx_t k = 0; k <= lim; ++k) {
        seq_t b1 = binom_mul(n - 2*k - 1, k);
        seq_t b2 = binom_mul(2*n - 2*k + 1, n - 3*k);
        seq_t denom = 2*n - 2*k + 1;
        seq_t term = b1 * b2;
        term /= denom;                 /* exact division */
        sum += term;
    }
    return sum;
}
