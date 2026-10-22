/*
 * A307939  Number of (undirected) Hamiltonian paths in the n-dipyramidal
 *          graph.
 * https://oeis.org/A307939
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^3 - 40*n^2 + 62*n for n > 3; a(3)=36
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 814, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)36;
    if (n < 3) return (seq_t)0;   /* undefined for n < 3, return 0 */

    seq_t tn = (seq_t)n;
    seq_t res = 8 * tn * tn * tn; // 8*n^3
    res -= 40 * tn * tn;          // -40*n^2
    res += 62 * tn;               // +62*n
    return res;
}
