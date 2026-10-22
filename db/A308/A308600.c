/*
 * A308600  Number of (not necessarily maximal) cliques in the n X n antelope
 *          graph.
 * https://oeis.org/A308600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2-28*n+49 for n >= 3; a(1)=2, a(2)=5
 * Range:     u8 n <= 9, u16 n <= 117, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset below not needed for this sequence
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    seq_t nn = (seq_t)n;
    return 5*nn*nn - 28*nn + 49;
}
