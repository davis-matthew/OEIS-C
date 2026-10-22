/*
 * A392531  Detour index of the n-necklace graph.
 * https://oeis.org/A392531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(2*n) = 2*n*(96*n^2 + 8*n - 11); a(2*n+1) =
 *            2*(2*n+1)*(48*n^2 + 52*n + 9)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 563, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* If n is even: n = 2*m  */
    if ((n & (idx_t)1) == (idx_t)0) {
        seq_t m = (seq_t)(n >> 1);           /* m = n/2   */
        return (seq_t)2 * m *
               ((seq_t)96 * m * m + (seq_t)8 * m - (seq_t)11);
    }
    /* If n is odd: n = 2*m+1 */
    {
        seq_t m = (seq_t)(n >> 1);           /* floor(n/2) */
        return (seq_t)2 * ((seq_t)2 * m + (seq_t)1) *
               ((seq_t)48 * m * m + (seq_t)52 * m + (seq_t)9);
    }
}
