/*
 * A330432  Number of permutations sigma of [n] such that k * sigma(k) >= n
 *          for 1 <= k <= n.
 * https://oeis.org/A330432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (n - k + 2 - ceil(n/k))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product is taken over k = 1, 2, ..., n.
   For n = 0 the empty product is defined as 1. */

#include <stdbool.h>   /* for bool, but not strictly needed */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition of the empty product */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        /* ceil(n/k) = (n + k - 1) / k  */
        seq_t ceil_nk = ((seq_t)n + (seq_t)k - (seq_t)1) / (seq_t)k;
        seq_t term = (seq_t)n - (seq_t)k + (seq_t)2 - ceil_nk;

        /* All terms are ≥ 1 by the underlying combinatorial reasoning,
           so multiplication is safe. */
        result *= term;
    }
    return result;
}
