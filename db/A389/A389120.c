/*
 * A389120  a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) with a(0)=0,
 *          a(1)=1, a(2)=2, a(3)=7.
 * https://oeis.org/A389120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) with a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)7;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)2;
    seq_t a3 = (seq_t)7;
    seq_t an;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = (seq_t)2 * a3 + (seq_t)2 * a2 + (seq_t)2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
