/*
 * A298029  Coordination sequence of Dual(3.4.6.4) tiling with respect to a
 *          trivalent node.
 * https://oeis.org/A298029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n = 0 → 1 n = 1 → 3 n = 2 → 6 n = 3 → 12 n = 4 → 18 n >=
 *            5: if n even → 9*n - 15 if n odd → 9*n - 12
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 6;
    if (n == 3) return 12;
    if (n == 4) return 18;

    /* For n >= 5 use the linear formula */
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0)      /* even */
        return 9 * nn - 15;
    else                   /* odd */
        return 9 * nn - 12;
}
