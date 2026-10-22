/*
 * A307685  Clique covering number of the n-Sierpinski gasket graph.
 * https://oeis.org/A307685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*3^(n-2) + (-1)^n + 4)/4 for n > 1; a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)17;

    seq_t a2 = (seq_t)3, a3 = (seq_t)6, a4 = (seq_t)17;
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = a4 * (seq_t)3 + a3 - a2 * (seq_t)3;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
