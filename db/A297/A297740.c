/*
 * A297740  The number of distinct positions on an infinite chessboard
 *          reachable by the (2,3)-leaper (or zebra) in at most n moves.
 * https://oeis.org/A297740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*n^2 - 38*n + 13 for n >= 7. Recurrence: a(n) =
 *            3*a(n-1) - 3*a(n-2) + a(n-3) for n > 9. Initial terms: a(0)=1,
 *            a(1)=9, a(2)=41, a(3)=129, a(4)=321, a(5)=625, a(6)=997
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {1, 9, 41, 129, 321, 625, 997};
    if (n < 7u) return init[(unsigned)n];
    unsigned long long nn = (unsigned long long)n;
    unsigned long long res = 34ULL * nn * nn - 38ULL * nn + 13ULL;
    return (seq_t)res;
}
