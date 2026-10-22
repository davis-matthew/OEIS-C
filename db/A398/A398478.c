/*
 * A398478  a(n) = Sum_{j=0..n} binomial(n + j, j) * n^(n - j) / (j + 1). Row
 *          sums of A398477.
 * https://oeis.org/A398478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} binomial(n + j, j) * n^(n - j) / (j + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A398478.  All arithmetic uses the generic integer
   type seq_t, computing each term iteratively to keep intermediate results
   as small as possible.  The recurrence T_j = T_{j-1} * (n + j) / (n * (j + 1))
   allows every step to stay integral. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

/* Compute A398478. */
seq_t a(idx_t n)
{
    /* Handle the special case n = 0.  By convention 0^0 is taken as 1. */
    if (n == 0)
        return 1;

    /* The first term of the sum: T_0 = n^n. */
    seq_t sum = big_pow((seq_t)n, (seq_t)n);
    seq_t term = sum;

    /* Generate subsequent terms T_j for j = 1 .. n. */
    for (idx_t j = 1; j <= n; ++j)
    {
        /* Ratio: T_j = T_{j-1} * (n + j) / (n * (j + 1)). */
        seq_t num = (seq_t)n + (seq_t)j;                 /* n + j          */
        seq_t den = (seq_t)n * ((seq_t)j + 1);           /* n * (j + 1)    */

        /* Cancel common factors between the running term and the denominator
           to prevent overflow. */
        seq_t g1 = gcd(term, den);
        term /= g1;
        den  /= g1;

        /* Cancel any remaining common factors between num and den. */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den  /= g2;  /* After the two cancellations, den should be 1. */

        /* Update the term and add it to the running sum. */
        term *= num;  /* term now reduced, product fits in seq_t. */
        /* If den is not 1, divide by it; mathematically it should be 1. */
        if (den != 1)
            term /= den;

        sum += term;
    }

    return sum;
}
