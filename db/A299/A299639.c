/*
 * A299639  a(0) = 0, and for n > 0, (a(n)) gives the indices n for which d(n)
 *          > d(k) for k < n, where d is the difference sequence of (cos k +
 *          sin k).
 * https://oeis.org/A299639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=4, a(2)=5, and for n>=3
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;
    idx_t k = n - 3;
    return (seq_t)52168 + (seq_t)710 * (seq_t)k;
}
