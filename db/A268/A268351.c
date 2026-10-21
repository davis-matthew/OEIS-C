/*
 * A268351  a(n) = 3*n*(9*n - 1)/2.
 * https://oeis.org/A268351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(9*n - 1)/2
 * Range:     u8 n <= 3, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t x = (seq_t)n;

    /* compute the terms of the formula */
    seq_t term1   = (seq_t)3 * x;          /* 3*n */
    seq_t term2   = (seq_t)9 * x - (seq_t)1; /* 9*n - 1 */
    seq_t product = term1 * term2;         /* 3*n*(9*n-1) */

    /* divide by 2 – the product is always even */
    seq_t result = product / (seq_t)2;

    return result;
}
