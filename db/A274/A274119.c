/*
 * A274119  a(n) = (Product_{i=0..4}(i*n+2) -
 *          Product_{i=0..4}(-i*n-1))/(4*n+3).
 * https://oeis.org/A274119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = (Product_{i=0..4}(i*n+2) -
 *            Product_{i=0..4}(-i*n-1))/(4*n+3) = 11 + 42*n + 49*n^2 + 18*n^3
 *            (valid for all n >= 0)
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 619, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* Compute 49*x^2 */
    seq_t x2 = x * x;         /* n^2 */
    seq_t term3 = 49 * x2;    /* 49*n^2 */

    /* Compute 18*x^3 */
    seq_t x3 = x2 * x;        /* n^3 */
    seq_t term4 = 18 * x3;    /* 18*n^3 */

    /* Compute 42*n and add the constants */
    seq_t term2 = 42 * x;     /* 42*n */

    return 11 + term2 + term3 + term4;
}
