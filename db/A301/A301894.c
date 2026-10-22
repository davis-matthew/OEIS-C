/*
 * A301894  Number of real lines on a smooth real cubic surface.
 * https://oeis.org/A301894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 2*n^2 - 2*n + 3
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the unsigned index to seq_t first */
    seq_t nn = (seq_t)n;
    /* compute 2*nn*nn - 2*nn + 3 */
    seq_t res = nn * nn;   /* nn^2          */
    res = res * 2;         /* 2*nn^2        */
    res -= nn * 2;         /* -2*nn         */
    res += 3;              /* +3            */
    return res;
}
