/*
 * A366023  Expansion of (1/x) * Series_Reversion( x*(1-x)*(1+x^4) ).
 * https://oeis.org/A366023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} (-1)^k * binomial(n+k,
 *            n) * binomial(2*n-4*k, n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* ----- Helper: binomial calculation ----------------------------------- */
/* Computes C(n,k) for 0 <= k <= n using only seq_t arithmetic.       */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)           return (seq_t)0;
    if (k > n - k)       k = n - k;          /* use symmetry */

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* result = result * (n - k + i) / i; division is exact.   */
        result = (result * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return result;
}

/* ----- Main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0) = 1; the general expression also gives this value. */
    idx_t maxk = n / 4;
    seq_t pos = (seq_t)0;   /* sum of terms with sign +1      */
    seq_t neg = (seq_t)0;   /* sum of terms with sign -1      */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binomial(n + k, n);
        seq_t c2 = binomial(2 * n - 4 * k, n);
        seq_t prod = c1 * c2;

        if (k & 1) {
            neg += prod;
        } else {
            pos += prod;
        }
    }

    /* pos >= neg for all n >= 0; difference is the numerator.     */
    seq_t diff = pos - neg;
    seq_t denom = (seq_t)n + 1;
    return diff / denom;
}
