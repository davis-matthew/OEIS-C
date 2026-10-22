/*
 * A357508  a(n) = binomial(4*n,2*n) - 2*binomial(4*n,n).
 * https://oeis.org/A357508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(4*n,2*n) - 2*binomial(4*n,n)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 16, i128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Multiplicative binomial coefficient:
   computes C(n,k) exactly using seq_t arithmetic. */
static seq_t binom_multiply(idx_t n, idx_t k)
{
    /* Use the smaller k for fewer iterations. */
    if (k > n - k)
        k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* Numerator term: n - k + i. */
        seq_t num = (seq_t)(n - k + i);
        /* Denominator term: i. */
        seq_t den = (seq_t)i;

        /* Reduce by gcd before multiplication to keep numbers small. */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(result, den);
        result /= g;
        den /= g;

        result = result * num;
        if (den != 1)
            result = result / den;   /* den should be 1 after reductions, but keep it safe. */
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Safety: convert n to seq_t only when needed. */
    idx_t four = n * 4;
    idx_t two  = n * 2;

    seq_t val1 = binom_multiply(four, two);   /* binomial(4*n, 2*n) */
    seq_t val2 = binom_multiply(four, n);     /* binomial(4*n, n)  */

    seq_t two_val2 = val2 + val2;             /* 2 * binomial(4*n, n) */
    return val1 - two_val2;
}
