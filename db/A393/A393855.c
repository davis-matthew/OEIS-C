/*
 * A393855  Sum of the distinct entries in the central row and central column
 *          of the (2*n + 1) X (2*n + 1) square array formed by the integers
 *          1, 2, ..., (2*n + 1)^2 in order.
 * https://oeis.org/A393855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = (4*n + 1)*(2*n^2 + 2*n + 1)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 812, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type explicitly to avoid
       unintended integer promotion issues. */
    seq_t ns = (seq_t)n;

    /* Compute the two parts of the product separately. */
    seq_t part1 = 4 * ns + 1;               /* 4*n + 1  */
    seq_t n_sq  = ns * ns;                 /* n^2      */
    seq_t part2 = 2 * n_sq + 2 * ns + 1;    /* 2*n^2 + 2*n + 1 */

    return part1 * part2;
}
