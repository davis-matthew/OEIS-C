/*
 * A308599  Number of (not necessarily maximal) cliques in the n-alternating
 *          group graph.
 * https://oeis.org/A308599
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (4*n - 5)*n!/6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2, but we return 0 for
       n < 2 as the offset value is not used. */
    if (n < 2) return 0;

    /* Compute n! using the supplied factorial routine. */
    seq_t nf = factorial(n);

    /* Compute (4*n - 5) as a seq_t to avoid mixing types. */
    seq_t coeff = (seq_t)(4 * n - 5);

    /* Multiply and divide by 6, using integer division that truncates. */
    seq_t prod = coeff * nf;
    seq_t res  = prod / 6 + 1;

    return res;
}
