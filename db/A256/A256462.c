/*
 * A256462  Double sum of the product of two binomials with even arguments.
 * https://oeis.org/A256462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n^2 + 4*n + 1)*Catalan(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the catalan helper from the utils directory. */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Compute the factor (2*n^2 + 4*n + 1) using seq_t arithmetic   */
    seq_t nn = (seq_t)n;
    seq_t factor = (seq_t)2 * nn * nn + (seq_t)4 * nn + (seq_t)1;

    /* Compute Catalan(2*n).  The helper catalan is provided.       */
    seq_t catal = catalan(2 * n);

    /* Return the product. */
    return factor * catal;
}
