/*
 * A328236  The least m > 1 such that the arithmetic derivative of m*n is a
 *          multiple of the arithmetic derivative of n.
 * https://oeis.org/A328236
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m>1 such that (m*n)' is a multiple of n'
 *            (arithmetical derivative is defined by (p)' = 1 for prime p and
 *            (xy)' = x y' + x' y)
 * Range:     u8 n <= 17, u16 n <= 327, u32 n <= 9802, u64 n <= 9802,
 *            u128 n <= 9802, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute arithmetic derivative of x:
   D(x) = x * sum_{p|x} e_p / p  = sum_{p|x} (x/p) * e_p   */
static seq_t arith_derivative_ui(uint64_t n, const uint64_t *pr, const uint64_t *ex, size_t cnt)
{
    seq_t d = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = pr[i];
        uint64_t exp = ex[i];
        d += (seq_t)(n / p) * (seq_t)exp;
    }
    return d;
}

/* Merge two sorted lists of prime factors (primes ascending) and
   compute derivative of n * m. */
static seq_t derivative_nm(uint64_t n, uint64_t m,
                           const uint64_t *pr_n, const uint64_t *ex_n, size_t cnt_n,
                           const uint64_t *pr_m, const uint64_t *ex_m, size_t cnt_m)
{
    seq_t mn = (seq_t)n * (seq_t)m;
    seq_t d_nm = 0;
    size_t i = 0, j = 0;

    while (i < cnt_n || j < cnt_m)
    {
        uint64_t p, exp;
        if (j >= cnt_m || (i < cnt_n && pr_n[i] < pr_m[j]))
        {
            p   = pr_n[i];
            exp = ex_n[i];
            ++i;
        }
        else if (i >= cnt_n || (j < cnt_m && pr_m[j] < pr_n[i]))
        {
            p   = pr_m[j];
            exp = ex_m[j];
            ++j;
        }
        else   /* equal primes */
        {
            p   = pr_n[i];
            exp = ex_n[i] + ex_m[j];
            ++i; ++j;
        }
        d_nm += (mn / p) * exp;
    }
    return d_nm;
}

seq_t a(idx_t n)
{
    /* n >= 2 by correctness contract */
    uint64_t num_n = (uint64_t)n;

    /* Factor n once */
    uint64_t pr_n[64], ex_n[64];
    size_t cnt_n;
    factor_u64(num_n, pr_n, ex_n, &cnt_n);
    seq_t dn = arith_derivative_ui(num_n, pr_n, ex_n, cnt_n);

    /* Search for smallest m > 1 satisfying the condition */
    for (uint64_t m = 2; ; ++m)
    {
        uint64_t num_m = m;
        uint64_t pr_m[64], ex_m[64];
        size_t cnt_m;
        factor_u64(num_m, pr_m, ex_m, &cnt_m);

        seq_t d_nm = derivative_nm(num_n, num_m,
                                   pr_n, ex_n, cnt_n,
                                   pr_m, ex_m, cnt_m);

        if (d_nm % dn == 0)
            return (seq_t)m;
    }
}
