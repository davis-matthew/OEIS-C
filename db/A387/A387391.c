/*
 * A387391  a(n) = 49*2^n - 3*n - 18.
 * https://oeis.org/A387391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*2^n - 3*n - 18
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 0; i < n; ++i)
        val <<= 1;        /* compute 2^n */
    val *= 49;           /* 49*2^n */
    seq_t sub = (seq_t)n * 3 + 18; /* 3*n + 18 */
    return val - sub;
}
