/*
 * A367414  Expansion of (1/x) * Series_Reversion( x * (1-x-x^4/(1-x)^2) ).
 * https://oeis.org/A367414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} binomial(n+k,k) *
 *            binomial(2*n-k,n-4*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* multiplicative binomial coefficient: C(n,k) */
static seq_t binom_mult(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        res *= num;
        res /= (seq_t)i;  /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t maxk = n / (idx_t)4;
    seq_t sum = (seq_t)0;

    for (idx_t k = (idx_t)0; k <= maxk; ++k)
    {
        seq_t term1 = binom_mult(n + k, k);
        seq_t term2 = binom_mult((idx_t)(2 * n - k), n - (idx_t)(4 * k));
        sum += term1 * term2;
    }
    return sum / ((seq_t)(n + (idx_t)1));
}
