/*
 * A273365  Numbers k such that 10*k+4 is a perfect square.
 * https://oeis.org/A273365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 10*n^2 + 4*n, a(2n-1) = 10*n^2 - 4*n, a(0)=0
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* All operations are performed in seq_t to support arbitrary precision. */
    seq_t k;
    if ((n & 1) == 0)          /* n is even: use a(2k) = 10*k^2 + 4*k */
    {
        k = (seq_t)(n >> 1);
        return (seq_t)10 * k * k + (seq_t)4 * k;
    }
    else                       /* n is odd: use a(2k-1) = 10*k^2 - 4*k  (k = (n+1)/2) */
    {
        k = (seq_t)((n + 1) >> 1);
        return (seq_t)10 * k * k - (seq_t)4 * k;
    }
}
