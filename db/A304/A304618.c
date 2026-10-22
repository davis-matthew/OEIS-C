/*
 * A304618  a(n) = 108*n^2 - 228*n + 114 (n>=2).
 * https://oeis.org/A304618
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 108*n^2 - 228*n + 114 (n>=2)
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t sn = (seq_t)n;
    seq_t res = 108 * (sn * sn) - 228 * sn + 114;
    return res;
}
