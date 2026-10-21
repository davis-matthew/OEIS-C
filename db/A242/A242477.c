/*
 * A242477  a(n) = floor(3*n^2/4).
 * https://oeis.org/A242477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n^2/4)
 * Range:     u8 n <= 18, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n >> 1;                     /* floor(n/2) */
    seq_t qq = (seq_t)q * (seq_t)q;        /* q*q  */
    seq_t res = (seq_t)3 * qq;             /* 3*q^2 */
    if (n & 1)                              /* if n is odd add 3*q */
        res += (seq_t)3 * (seq_t)q;
    return res;
}
