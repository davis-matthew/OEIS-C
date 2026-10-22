/*
 * A302302  Number of triples (i,j,k) such that i+j+k > 0 with -n <= i,j,k <=
 *          n.
 * https://oeis.org/A302302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3 + 9*n + 8*n^2)/2
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the idx_t argument to seq_t before any arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute 3 + 9*n + 8*n^2, casting every intermediate result to seq_t */
    seq_t temp = (seq_t)3
               + (seq_t)9 * nn
               + (seq_t)8 * nn * nn;

    /* The product nn * temp is always even, so division by 2 is exact */
    return (nn * temp) / (seq_t)2;
}
