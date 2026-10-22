/*
 * A300757  Number of {0,1} n X n matrices with at least one zero row or
 *          column.
 * https://oeis.org/A300757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2) - Σ_{i=0}^n (-1)^i * C(n,i) * (2^(n-i)-1)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   binomial, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A300757: Number of {0,1} n×n matrices with at least one zero row or column. */

#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Total number of binary n×n matrices */
    seq_t total = pow2(n * n);

    /* Accumulate the inclusion–exclusion sum in two parts to avoid
       signed arithmetic on unsigned seq_t. */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* binomial coefficient C(n,i) */
        seq_t comb = binomial(n, i);

        /* (2^(n-i)-1)^n */
        seq_t base = pow2(n - i);
        base = base - 1;                     /* base = 2^(n-i)-1, can be 0 when i=n */

        seq_t pow_base_n = pow_seq(base, (int)n); /* n fits in int for our range */

        /* term = C(n,i) * (2^(n-i)-1)^n */
        seq_t term = comb * pow_base_n;

        /* Split according to the sign (-1)^i */
        if ((i & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    /* Summation result: Σ (-1)^i C(n,i) (2^(n-i)-1)^n */
    seq_t inclusion_sum = sum_pos - sum_neg;    /* sum_pos ≥ sum_neg always */

    /* Desired count: total - inclusion_sum */
    return total - inclusion_sum;
}
