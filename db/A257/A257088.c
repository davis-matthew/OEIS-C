/*
 * A257088  a(2*n) = 4*n if n>0, a(2*n + 1) = 2*n + 1, a(0) = 1.
 * https://oeis.org/A257088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(0) = 1, a(2*n) = 4*n, a(2*n + 1) = 2*n + 1
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    if ((n & 1) == 0) {
        return (seq_t)n * 2;   /* a(2*k) = 4*k = 2 * (2*k) = 2*n */
    } else {
        return (seq_t)n;       /* a(2*k+1) = 2*k+1 = n */
    }
}
