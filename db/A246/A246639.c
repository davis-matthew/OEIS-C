/*
 * A246639  Sequence a(n) = 3 + 5*A001519(n+1) appearing in a certain three
 *          circle touching problem, together with A246638.
 * https://oeis.org/A246639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) initial values: a(0)=8,
 *            a(1)=13, a(2)=28
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)8;
    if (n == 1u) return (seq_t)13;
    if (n == 2u) return (seq_t)28;

    seq_t a0 = (seq_t)8;
    seq_t a1 = (seq_t)13;
    seq_t a2 = (seq_t)28;
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = (seq_t)4 * a2 - (seq_t)4 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
