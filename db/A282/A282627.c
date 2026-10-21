/*
 * A282627  Exponential expansion of the square of the real root y = y(x) of
 *          y^3 - 3*x*y - 1.
 * https://oeis.org/A282627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2 * Product_{j=1..n-1}(n + 2 - 3*j) (n >= 1, a(0)=1)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 13, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n == 0) {
        return 1;
    }

    seq_t prod = 1;
    seq_t sn   = (seq_t)n;   /* make n a seq_t for arithmetic */

    /* Compute the product.  The loop runs for j = 1 .. n-1. */
    for (idx_t j = 1; j < n; ++j) {
        seq_t term = sn + 2 - 3 * (seq_t)j;
        prod *= term;
    }

    /* Multiply by 2 as per the definition. */
    return prod * 2;
}
