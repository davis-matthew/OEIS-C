/*
 * A288429  a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4), where a(0) = 2, a(1) =
 *          4, a(2) = 5, a(3) = 6.
 * https://oeis.org/A288429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) with a(0)=2, a(1)=4,
 *            a(2)=5, a(3)=6
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 56, u64 n <= 114,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)6;

    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)5;
    seq_t a3 = (seq_t)6;
    seq_t an = a3;

    for (idx_t i = 4; i <= n; ++i) {
        an = 2 * a3 - a2 + a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
