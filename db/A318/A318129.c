/*
 * A318129  Number of sets of nonempty subsets of {1,...,n} with intersection
 *          {}.
 * https://oeis.org/A318129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{2^n-1} + 1 - Σ_{m=1..n} (-1)^{m+1}·C(n,m)·2^{2^{n-m}}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include "../utils/binom.h"
#include "../utils/pow_uint.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pos_sum = 0;   /* sum of terms with positive sign (m odd) */
    seq_t neg_sum = 0;   /* sum of terms with negative sign (m even) */

    for (idx_t m = 1; m <= n; ++m)
    {
        /* term = C(n,m) * 2^{2^{n-m}} */
        seq_t bin = binom(n, m);          /* binomial coefficient */
        seq_t exp = pow_uint(2, n - m);   /* 2^{n-m} as seq_t */
        seq_t pow2 = big_pow( (seq_t)2, exp ); /* 2^{2^{n-m}} */
        seq_t term = bin * pow2;

        if (m & 1)          /* m odd -> positive sign */
            pos_sum += term;
        else                /* m even -> negative sign */
            neg_sum += term;
    }

    /* total = 2^{2^n-1} */
    seq_t exp_total = pow_uint(2, n);          /* 2^n */
    exp_total -= (seq_t)1;                    /* 2^n - 1 */
    seq_t total = big_pow( (seq_t)2, exp_total ); /* 2^{2^n-1} */

    seq_t sum = pos_sum - neg_sum;            /* Σ (-1)^{m+1}C(n,m)2^{2^{n-m}} */

    seq_t result = total + (seq_t)1 - sum;    /* a(n) */

    return result;
}
