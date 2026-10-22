/*
 * A383854  a(n) = 4*n^3 + 5*n - 1.
 * https://oeis.org/A383854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 + 5*n - 1
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type for exact arithmetic. */
    seq_t nn = (seq_t)n;

    /* Compute n^3 as (n * n) * n. */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* Evaluate the polynomial: 4*n^3 + 5*n - 1. */
    seq_t result = (seq_t)4 * n3 + (seq_t)5 * nn - (seq_t)1;

    return result;
}
