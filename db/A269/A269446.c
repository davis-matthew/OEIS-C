/*
 * A269446  a(n) = n*(n^6 + n^3 + 1)*(n^6 - n^3 + 1)*(n^2 + n + 1)*(n^2 - n +
 *          1)*(n + 1) + 1.
 * https://oeis.org/A269446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^6 + n^3 + 1)*(n^6 - n^3 + 1)*(n^2 + n + 1)*(n^2 - n
 *            + 1)*(n + 1) + 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 11, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x      = (seq_t)n;
    seq_t x_plus1= x + 1;
    seq_t x2     = x * x;
    seq_t x3     = x2 * x;
    seq_t x6     = x3 * x3;

    /* individual polynomial components */
    seq_t term1  = x6 + x3 + 1;   /* n^6 + n^3 + 1  */
    seq_t term2  = x6 - x3 + 1;   /* n^6 - n^3 + 1  */
    seq_t term3  = x2 + x + 1;    /* n^2 + n + 1    */
    seq_t term4  = x2 - x + 1;    /* n^2 - n + 1    */

    /* accumulate product */
    seq_t prod   = x;
    prod *= x_plus1;
    prod *= term3;
    prod *= term4;
    prod *= term1;
    prod *= term2;

    prod += 1;
    return prod;
}
