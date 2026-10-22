/*
 * A392334  a(2*n) = 2*n+1, a(2*n+1) = (2*n+1)^2.
 * https://oeis.org/A392334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 2*n+1, a(2*n+1) = (2*n+1)^2 (OEIS A392334)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* odd index: a(n) = n^2 */
    if (n & 1U)
    {
        seq_t nn = (seq_t)n;
        return nn * nn;
    }
    /* even index: a(n) = 2*(n/2)+1 */
    seq_t half = (seq_t)(n >> 1);
    return half * 2 + 1;
}
