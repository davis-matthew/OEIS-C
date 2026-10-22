/*
 * A390087  a(n) is the smallest index k such that A390085(k) = n.
 * https://oeis.org/A390087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 1, a(2) = 4, a(n) = ⌊(n²+2n-2)/2⌋ for n ≥ 3
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    seq_t nn = (seq_t)n;
    /* compute (n*n + 2*n - 2) / 2 using integer truncation */
    return (nn * nn + (seq_t)2 * nn - (seq_t)2) / (seq_t)2;
}
