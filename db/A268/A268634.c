/*
 * A268634  Number of n X 3 0..2 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling two exactly
 *          once.
 * https://oeis.org/A268634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 29*a(n-2) + 20*a(n-3) - 4*a(n-4) (n>=5)
 *            Initial values: a(1)=12, a(2)=120, a(3)=840, a(4)=5178
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)120;
    if (n == 3) return (seq_t)840;
    if (n == 4) return (seq_t)5178;

    seq_t a1 = (seq_t)12;
    seq_t a2 = (seq_t)120;
    seq_t a3 = (seq_t)840;
    seq_t a4 = (seq_t)5178;
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)10 * a4 + (seq_t)20 * a2;
        next -= (seq_t)29 * a3;
        next -= (seq_t)4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
