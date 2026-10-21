/*
 * A283394  a(n) = 3*n*(3*n + 7)/2 + 4.
 * https://oeis.org/A283394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(3*n + 7)/2 + 4
 * Range:     u8 n <= 4, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;              /* cast index to sequence type */
    seq_t val = N * (3 * N + 7);     /* N * (3N + 7)    */
    val *= 3;                        /* multiply by 3    */
    val /= 2;                        /* divide by 2      */
    val += 4;                        /* add the constant */
    return val;
}
