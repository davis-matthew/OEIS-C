/*
 * A336522  a(n) is the coefficient of x^(n^2) in expansion of ( (1 + x)/(1 -
 *          x) )^n.
 * https://oeis.org/A336522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^k * binomial(n,k) * binomial(n^2-1, k-1)
 *            with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static inline seq_t binom_next(seq_t prev, idx_t n, idx_t k)
{
    /* compute binomial(n,k) from binomial(n,k-1) = prev */
    /* prev * (n-k+1) / k, with cancellation to avoid overflow */
    seq_t num = prev * (seq_t)(n - k + 1);
    seq_t den = (seq_t)k;
    seq_t g   = gcd(num, den);
    num /= g;  den /= g;
    return num / den;   /* den should now be 1 */
}

static inline seq_t binomN_small_next(seq_t prev, idx_t N, idx_t r)
{
    /* compute binomial(N,r+1) from binomial(N,r) = prev */
    seq_t num = prev * (seq_t)(N - r);
    seq_t den = (seq_t)(r + 1);
    seq_t g   = gcd(num, den);
    num /= g;  den /= g;
    return num / den;   /* den should now be 1 */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t N_minus_1 = n * n - 1;    /* n^2-1, fits in idx_t for n<=10000 */
    seq_t sum   = 0;
    seq_t pow2  = 2;                /* 2^1 */
    seq_t B_prev = 1;               /* binomial(n,0) */
    seq_t C_prev = 1;               /* binomial(N-1,0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t B_k = (k == 1) ? (seq_t)n : binom_next(B_prev, n, k);
        seq_t C_k = (k == 1) ? (seq_t)1
                            : binomN_small_next(C_prev, N_minus_1, k - 2);

        /* term = 2^k * B_k * C_k */
        seq_t term = B_k;
        term *= C_k;
        term *= pow2;
        sum += term;

        B_prev = B_k;
        C_prev = C_k;
        pow2 *= 2;
    }
    return sum;
}
