/*
 * A332936  Number of blue nodes in n-th power graph W exponentiation of a
 *          cycle graph with 7 blue nodes and 1 green node.
 * https://oeis.org/A332936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=7, a(1)=51, a(n)=8*a(n-1)-3*a(n-2) for n>1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)51;

    seq_t a0 = (seq_t)7;
    seq_t a1 = (seq_t)51;
    seq_t a2 = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a2 = (seq_t)8 * a1 - (seq_t)3 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a2;
}
