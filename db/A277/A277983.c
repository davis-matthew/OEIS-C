/*
 * A277983  a(n) = 54*n^2 - 78*n + 36.
 * https://oeis.org/A277983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 54*n^2 - 78*n + 36
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;              /* Promote the index to the result type. */
    seq_t res = (seq_t)54 * N * N;   /* 54*n^2  */
    res = res + (seq_t)36;           /* +36    */
    return res - (seq_t)78 * N;      /* -78*n  */
}
