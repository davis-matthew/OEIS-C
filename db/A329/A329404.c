/*
 * A329404  Interleave 2*n*(3*n-1), (2*n+1)*(6*n+1) for n >= 0.
 * https://oeis.org/A329404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 2*k*(3*k-1), a(2k+1) = (2*k+1)*(6*k+1) for k >= 0
 * Range:     u8 n <= 10, u16 n <= 148, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;              /* k = floor(n/2) */
    if ((n & 1) == 0)              /* even index: a(2k) */
    {
        return (seq_t)2 * (seq_t)k * ((seq_t)3 * k - 1);
    }
    else                            /* odd index: a(2k+1) */
    {
        return ((seq_t)2 * k + 1) * ((seq_t)6 * k + 1);
    }
}
