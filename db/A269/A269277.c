/*
 * A269277  Number of 2 X n 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          exactly once.
 * https://oeis.org/A269277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=108, a(3)=1368, a(4)=13896,
 *            a(n)=14*a(n-1)-49*a(n-2) for n>4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)108;
    if (n == 3)
        return (seq_t)1368;
    if (n == 4)
        return (seq_t)13896;

    seq_t a_prev2 = (seq_t)1368; /* a(3) */
    seq_t a_prev1 = (seq_t)13896;/* a(4) */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = (seq_t)14 * a_prev1 - (seq_t)49 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}
