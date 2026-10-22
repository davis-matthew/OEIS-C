/*
 * A370238  a(n) = n*(3*n + 23)/2.
 * https://oeis.org/A370238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n + 23)/2
 * Range:     u8 n <= 9, u16 n <= 205, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure the index is converted to the result type before any
       arithmetic so intermediate calculations stay within seq_t. */
    seq_t x = (seq_t)n;
    seq_t temp = x * 3 + 23;   /* 3*n + 23 */
    return x * temp / 2;      /* n*(3*n + 23)/2 */
}
