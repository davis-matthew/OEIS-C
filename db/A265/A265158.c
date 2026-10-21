/*
 * A265158  a(1) = 1, a(2*n) = 2*a(n), a(2*n+1) = floor(a(n)/2).
 * https://oeis.org/A265158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2*n) = 2 * a(n); a(2*n+1) = floor(a(n)/2)
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)                     /* base case */
        return (seq_t)1;
    if ((n & (idx_t)1) == 0)        /* n is even  -> a(2*n) = 2*a(n) */
        return (seq_t)2 * a(n >> 1);
    else                            /* n is odd   -> a(2*n+1) = floor(a(n)/2) */
        return a(n >> 1) / (seq_t)2;
}
