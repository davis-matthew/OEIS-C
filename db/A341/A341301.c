/*
 * A341301  a(n) = ceiling(n^2 - 7*n/3 + 19/3).
 * https://oeis.org/A341301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n^2 - 7*n/3 + 19/3)
 * Range:     u8 n <= 16, u16 n <= 257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: a(n) = (3*n*n - 7*n + 21) / 3  for n >= 1  */
seq_t a(idx_t n)
{
    /* Use idx_t for intermediate calculation to avoid overflow
       for the expected range of n.  The final value is converted
       to seq_t before returning. */
    idx_t nn = n;
    idx_t numer = 3 * nn * nn - 7 * nn + 21;
    return (seq_t)(numer / 3);
}
