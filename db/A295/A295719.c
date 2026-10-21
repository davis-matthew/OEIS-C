/*
 * A295719  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 1,
 *          a(1) = 3, a(2) = 6, a(3) = 10.
 * https://oeis.org/A295719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4);
   a(0)=1, a(1)=3, a(2)=6, a(3)=10 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)10;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)6;
    seq_t a3 = (seq_t)10;
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i)
    {
        ai = a3 + (seq_t)3 * a2 - (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
