/*
 * A368769  a(n) = (n!)^3 * Sum_{k=1..n} 1/(k!)^3.
 * https://oeis.org/A368769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 * a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t factor = i_seq * i_seq * i_seq;  /* i^3 */
        res = res * factor + 1;
    }
    return res;
}
