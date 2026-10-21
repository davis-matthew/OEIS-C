/*
 * A286814  Number of matchings in the n-helm graph.
 * https://oeis.org/A286814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4), a(0) = 2, a(1) =
 *            3, a(2) = 10, a(3) = 29
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)29;

    seq_t a0 = 2, a1 = 3, a2 = 10, a3 = 29, an;
    for (idx_t i = 4; i <= n; ++i) {
        an = 4 * a3 - 2 * a2 - 4 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
