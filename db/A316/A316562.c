/*
 * A316562  Koechel number for the works of W. A. Mozart rounded from age 11.
 * https://oeis.org/A316562
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = round(24.4*n - 252.8) (Koechel number for Mozart’s works)
 * Range:     u8 n <= 12, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 24.4 = 122/5, 252.8 = 1264/5  →  a(n) = round((122*n - 1264)/5)  */
    seq_t num = (seq_t)122 * n - (seq_t)1264;
    return (num + (seq_t)2) / (seq_t)5;   /* integer rounding by (num+2)/5 */
}
