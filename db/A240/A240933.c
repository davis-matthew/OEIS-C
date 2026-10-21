/*
 * A240933  a(n) = n^10 - n^9.
 * https://oeis.org/A240933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9*(n-1) = n^10 - n^9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* Compute n^9 by successive multiplication */
    seq_t pow = nn;                /* n^1 */
    for (idx_t i = 2; i <= 9; ++i) /* up to n^9 */
        pow *= nn;

    /* Now compute n^9*(n-1) */
    return pow * (nn - 1);
}
