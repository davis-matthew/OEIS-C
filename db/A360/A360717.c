/*
 * A360717  Number of unordered pairs of self-avoiding paths whose sets of
 *          nodes are disjoint subsets of a set of n points on a circle;
 *          one-node paths are allowed.
 * https://oeis.org/A360717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(5^(n-2) + 6*3^(n-2) + 9)/32
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (a(1) = 0) */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1)
        return (seq_t)0;

    /* Compute 5^(n-2) and 3^(n-2) via repeated multiplication */
    idx_t exp = n - 2;
    seq_t pow5 = (seq_t)1;
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        pow5 *= (seq_t)5;
        pow3 *= (seq_t)3;
    }

    /* term = 5^(n-2) + 6*3^(n-2) + 9 */
    seq_t term = pow5 + (seq_t)6 * pow3 + (seq_t)9;

    /* a_coef = n*(n-1) */
    seq_t a_coef = ((seq_t)n) * ((seq_t)(n - 1));

    /* Compute product with division by 32 without overflow for all n whose
       result fits into seq_t.  Split the term into quotient and remainder
       modulo 32. */
    seq_t q = term / (seq_t)32;
    seq_t r = term % (seq_t)32;

    seq_t part1 = a_coef * q;
    seq_t part2 = (a_coef * r) / (seq_t)32;

    return part1 + part2;
}
