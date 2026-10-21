/*
 * A294178  a(2n) = 2*n + 1, a(2n+1) = 6*n + 3.
 * https://oeis.org/A294178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 2*n + 1, a(2n+1) = 6*n + 3
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val;
    if ((n & 1) == 0)                /* even index */
    {
        idx_t k = n >> 1;            /* k = n/2 */
        val = (seq_t)k * ((seq_t)2) + ((seq_t)1);
    }
    else                            /* odd index */
    {
        idx_t k = (n - 1) >> 1;      /* k = (n-1)/2 */
        val = (seq_t)k * ((seq_t)6) + ((seq_t)3);
    }
    return val;
}
