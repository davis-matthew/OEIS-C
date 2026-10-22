/*
 * A386222  Number of 3-dimensional tilings of a 2 X 2 X (n+1) box with the
 *          two upper right cells removed, using 2 X 2 X 1 plates and 1 X 2 X
 *          1 dominos.
 * https://oeis.org/A386222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 9*a(n-2) - 14*a(n-3), a(0)=1, a(1)=5, a(2)=34
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast iterative computation using the linear recurrence */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)34;

    seq_t a0 = (seq_t)1, a1 = (seq_t)5, a2 = (seq_t)34, an;
    for (idx_t i = 3; i <= n; ++i) {
        an = 5 * a2 + 9 * a1 - 14 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
