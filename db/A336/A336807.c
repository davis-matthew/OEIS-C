/*
 * A336807  a(n) = (n!)^2 * Sum_{k=0..n} 4^(n-k) / (k!)^2.
 * https://oeis.org/A336807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * n^2 * a(n-1) + 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t res = (seq_t)1;           /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = (seq_t)4 * (seq_t)i * (seq_t)i;
        res = res * mult + (seq_t)1;
    }
    return res;
}
