/*
 * A374224  Integer part of the total Euclidean length of the shortest
 *          minimum-link polygonal chains joining all the nodes of the grid
 *          {0,1,...,n-1} X {0,1,...,n-1}.
 * https://oeis.org/A374224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 4 for n >= 6; special cases a(1)=0, a(2)=3,
 *            a(3)=12, a(4)=20, a(5)=28
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 3;
    if (n == 3) return 12;
    if (n == 4) return 20;
    if (n == 5) return 28;
    return (seq_t)n * (seq_t)n + 4;
}
