/*
 * A258807  a(n) = n^5 - 1.
 * https://oeis.org/A258807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 - 1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^5 in seq_t, then subtract 1. */
    seq_t x = (seq_t)n;      /* n */
    x *= x;                  /* n^2 */
    seq_t y = x * x;         /* n^4 */
    y *= (seq_t)n;           /* n^5 */
    return y - ((seq_t)1);
}
