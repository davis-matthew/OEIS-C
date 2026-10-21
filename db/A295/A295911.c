/*
 * A295911  Number of (not necessarily maximal) cliques in the n-Hanoi graph.
 * https://oeis.org/A295911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 1, a(1) = 8
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)8;
    seq_t res = 8;
    for (idx_t i = 2; i <= n; ++i) {
        res = res * 3 + 1;
    }
    return res;
}
