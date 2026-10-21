/*
 * A262140  The first of nine consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of eight
 *          consecutive positive integers.
 * https://oeis.org/A262140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) * Initial values: a(1)=20,
 *            a(2)=136, a(3)=812
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* outside OEIS offset, safe return */

    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)136;
    if (n == 3) return (seq_t)812;

    /* a0 = a(i-3), a1 = a(i-2), a2 = a(i-1)   */
    seq_t a0 = 20;
    seq_t a1 = 136;
    seq_t a2 = 812;
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        next = (seq_t)7 * a2 - (seq_t)7 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
