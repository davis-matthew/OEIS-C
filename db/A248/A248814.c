/*
 * A248814  a(n) = (6n)!/(6!^n).
 * https://oeis.org/A248814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6n)!/(6!^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the OEIS sequence A248814.
   a(n) = product_{k=1..n} binom(6k,6),   a(0) = 1. */
seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return 1;

    seq_t result = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute binom(6k,6) multiplicatively to avoid large intermediates. */
        seq_t comb = 1;
        /* The numerator is (6k-5)(6k-4)...(6k). */
        idx_t start = 6 * k - 5;   /* smallest term in the product */
        /* iterate i = 1..6, multiplying by (start + i - 1) and dividing by i */
        for (idx_t i = 1; i <= 6; ++i)
        {
            comb *= (seq_t)(start + i - 1);
            comb /= (seq_t)i;   /* exact division */
        }
        result *= comb;
    }

    return result;
}
