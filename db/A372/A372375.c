/*
 * A372375  Expansion of (1/x) * Series_Reversion( x * (1+x) / (1+x+x^3)^2 ).
 * https://oeis.org/A372375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(2*n+2,k) *
 *            binomial(n-k+1,n-3*k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplicative binomial coefficient */
static seq_t nCk(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    idx_t kk = k;
    if (kk > n / 2) kk = n - kk;          /* use symmetry to reduce loop count */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= kk; ++i)
    {
        /* res = res * (n - kk + i) / i */
        res = res * (seq_t)(n - kk + i);
        res = res / (seq_t)i;             /* division is exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = nCk(2 * n + 2, k);
        seq_t c2 = nCk(n - k + 1, n - 3 * k);
        sum += c1 * c2;
    }
    return sum / (seq_t)(n + 1);
}
