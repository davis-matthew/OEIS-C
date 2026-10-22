/*
 * A360801  Expansion of Sum_{k>0} (x / (1 - 2 * x^k))^k.
 * https://oeis.org/A360801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(n/d-1) * binomial(d+n/d-2, d-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute 2^exp in a type‑agnostic way */
static seq_t pow2_exp(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r = r * 2;
    return r;
}

/* Compute binomial(n, k) multiplicatively */
static seq_t binomial_seq(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

/* Compute the summand for a given divisor d of n */
static seq_t term_div(idx_t n, idx_t d)
{
    idx_t m = n / d;
    seq_t pow = pow2_exp(m - 1);
    seq_t bin = binomial_seq(d + m - 2, d - 1);
    return pow * bin;
}

/* Sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;
            sum += term_div(n, d1);
            if (d1 != d2)
                sum += term_div(n, d2);
        }
    }
    return sum;
}
