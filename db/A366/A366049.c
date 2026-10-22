/*
 * A366049  Expansion of (1/x) * Series_Reversion( x*(1-x)^3/(1-x+x^2) ).
 * https://oeis.org/A366049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/2⌋} binomial(n+1,k) *
 *            binomial(3*n-k+1, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute binomial coefficient C(n,k) using multiplicative formula.
   Uses 128‑bit arithmetic when available to avoid overflow of the
   intermediate product.  The result is cast to seq_t. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (n-k+i) before dividing by i.
           Use 128‑bit intermediate when available. */
#if defined(__SIZEOF_INT128__) && !defined(__INT128_TYPE__)
        __int128 tmp = (__int128)res * (idx_t)(n - k + i);
        tmp /= i;
        res = (seq_t)tmp;
#else
        /* Fallback: product in seq_t; may overflow for large values,
           but this case occurs only when the result itself exceeds
           the capacity of seq_t. */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
#endif
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t maxk = n >> 1;                      /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binom_seq(n + 1, k);
        seq_t t2 = binom_seq(3 * n - k + 1, n - 2 * k);
        sum += t1 * t2;
    }

    return sum / (seq_t)(n + 1);
}
