/*
 * A368725  a(n) = n! * Sum_{k=0..n} (-1)^(n-k) * k^n / k!.
 * https://oeis.org/A368725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..n} (-1)^(n-k) * k^n / k!
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation evaluates the sum directly.  For each n it
    computes n! by a simple loop, then iterates over k = 0 … n,
    maintaining the ratio n! / k! and the power k^n via the helper
    function big_pow.  All arithmetic is performed in the seq_t type. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static seq_t factorial_u64(idx_t n)
{
    seq_t f = 1;
    for (idx_t i = 1; i <= n; ++i)
        f *= (seq_t)i;
    return f;
}

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Compute n! */
    seq_t fact_n = factorial_u64(n);

    seq_t result = 0;
    seq_t ratio  = fact_n;          /* ratio = n! / k!  (initially k=0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t pow_k_n;

        if (k == 0)
        {
            /* 0^0 = 1, 0^n = 0 for n>0 */
            pow_k_n = (n == 0) ? (seq_t)1 : (seq_t)0;
        }
        else
        {
            pow_k_n = big_pow((seq_t)k, (seq_t)n);
        }

        seq_t term = pow_k_n * ratio;

        /* Apply sign (-1)^(n-k) */
        if ((n - k) & 1)
            term = -term;

        result += term;

        /* Update ratio to n!/(k+1)! for next iteration */
        if (k < n)
            ratio /= (seq_t)(k + 1);
    }

    return result;
}
