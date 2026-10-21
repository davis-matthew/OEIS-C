/*
 * A290061  a(n) = (1/24)*(n + 3)*(3*n^3 + 5*n^2 - 6*n + 16).
 * https://oeis.org/A290061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*(n+3)*(3*n^3 + 5*n^2 - 6*n + 16)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 196, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to avoid overflow issues. */
    seq_t nn = (seq_t)n;

    /* Compute n+3. */
    seq_t n_plus_3 = nn + (seq_t)3;

    /* Compute n^2 and n^3. */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* Compute the cubic polynomial: 3*n^3 + 5*n^2 - 6*n + 16. */
    seq_t poly = (seq_t)3 * n3
               + (seq_t)5 * n2
               - (seq_t)6 * nn
               + (seq_t)16;

    /* Multiply by (n+3). */
    seq_t product = n_plus_3 * poly;

    /* Divide by 24 (exact division). */
    return product / (seq_t)24;
}
