/*
 * A243645  Number of ways two L-tiles can be placed on an n X n square.
 * https://oeis.org/A243645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n < 2 a(n) = (n^4 - 4*n^3 - n^2 + 18*n - 16)/2 for
 *            n >= 2 (OEIS A243645)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 290, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the value is 0. */
    if (n < 2)
        return 0;

    /* Convert index to the sequence type. */
    seq_t nt = (seq_t)n;

    /* Compute powers of n. */
    seq_t n2 = nt * nt;
    seq_t n3 = n2 * nt;
    seq_t n4 = n3 * nt;

    /* Form positive and negative parts to avoid underflow.
       positive = n^4 + 18*n - 16
       negative = 4*n^3 + n^2
       numerator = positive - negative
       result = numerator / 2
    */
    seq_t positive = n4 + (18 * nt) - 16;
    seq_t negative = (4 * n3) + n2;

    seq_t numerator = positive - negative;
    return numerator / 2;
}
