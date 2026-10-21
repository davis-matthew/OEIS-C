/*
 * A297741  The number of distinct positions on an infinite chessboard
 *          reachable by the (3,4)-leaper in <= n moves.
 * https://oeis.org/A297741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n > 13 Initial values
 *            (obtained from OEIS): a(0)=1, a(1)=9, a(2)=41, a(3)=129,
 *            a(4)=321, a(5)=681, a(6)=1289, a(7)=2121, a(8)=3081, a(9)=4121,
 *            a(10)=5233, a(11)=6445, a(12)=7777, a(13)=9233
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[14] = {
        1, 9, 41, 129, 321, 681, 1289, 2121, 3081, 4121, 5233,
        6445, 7777, 9233
    };

    if (n <= 13)
        return init[n];

    /* Compute using the linear recurrence for n >= 14 */
    seq_t a0 = init[11]; /* a(11) */
    seq_t a1 = init[12]; /* a(12) */
    seq_t a2 = init[13]; /* a(13) */

    for (idx_t i = 14; i <= n; ++i) {
        seq_t an = 3 * a2 - 3 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}
