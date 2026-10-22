/*
 * A378933  Number of minimal edge cuts in the 3 X n grid graph.
 * https://oeis.org/A378933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*2^n - (2*n^3 + 9*n^2 + 73*n + 96)/6
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Compute the main term 16*2^n */
    seq_t term1 = (seq_t)16 * pow2(n);

    /* Compute polynomial part */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2 * n_seq;             /* n^3 */

    seq_t poly = (seq_t)2 * n3 + (seq_t)9 * n2 + (seq_t)73 * n_seq + (seq_t)96;
    seq_t poly_div = poly / (seq_t)6;  /* exact division */

    /* Final result */
    return term1 - poly_div;
}
