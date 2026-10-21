/*
 * A266883  Numbers of the form m*(4*m+1)+1, where m = 0,-1,1,-2,2,-3,3,...
 * https://oeis.org/A266883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*floor((n+1)/2)+1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t s  = ((seq_t)(n + 1)) / 2;        /* floor((n+1)/2) */
    return ((seq_t)2 * nn + 1) * s + 1;
}
