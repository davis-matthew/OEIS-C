/*
 * A331394  Number of ways of 4-coloring the Fibonacci square spiral tiling of
 *          n squares with colors introduced in order.
 * https://oeis.org/A331394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + 2*a(n-3) for n >= 7 a(1) = 1, a(2) = 1,
 *            a(3) = 1, a(4) = 2, a(5) = 3, a(6) = 5
 * Range:     u8 n <= 19, u16 n <= 37, u32 n <= 74, u64 n <= 147,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) return 1;
    if (n == 4) return 2;
    if (n == 5) return 3;
    if (n == 6) return 5;

    seq_t a_m3 = 2;   /* a(i-3)  */
    seq_t a_m2 = 3;   /* a(i-2)  */
    seq_t a_m1 = 5;   /* a(i-1)  */
    seq_t curr;

    for (idx_t i = 7; i <= n; ++i) {
        curr = a_m1 - a_m2 + 2 * a_m3;
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = curr;
    }
    return a_m1;
}
