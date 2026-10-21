/*
 * A290325  Number of minimal dominating sets (and maximal irredundant sets)
 *          in the complete tripartite graph K_{n,n,n}.
 * https://oeis.org/A290325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 3 for n > 1; a(1) = 3
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)3;
    seq_t nn = (seq_t)n;
    return 3 * nn * nn + 3;
}
