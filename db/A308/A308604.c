/*
 * A308604  Number of (not necessarily maximal) cliques in the n X n
 *          fiveleaper graph.
 * https://oeis.org/A308604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=5, a(3)=10, a(4)=17; a(n)=7*n^2-38*n+49 for n>4
 * Range:     u8 n <= 8, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 5;
    if (n == 3) return 10;
    if (n == 4) return 17;
    seq_t nn = (seq_t)n;
    return (seq_t)7 * nn * nn - (seq_t)38 * nn + (seq_t)49;
}
