/*
 * A269196  Number of n X 3 0..3 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling three no more than once.
 * https://oeis.org/A269196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 237*a(n-2) + 180*a(n-3) - 36*a(n-4), for n>5;
 *            Initial values: a(1)=60, a(2)=1284, a(3)=25572, a(4)=471492,
 *            a(5)=8314020
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first five terms directly */
    if (n == 1) return (seq_t)60;
    if (n == 2) return (seq_t)1284;
    if (n == 3) return (seq_t)25572;
    if (n == 4) return (seq_t)471492;
    if (n == 5) return (seq_t)8314020;

    /* Set up previous four terms */
    seq_t a2 = (seq_t)1284;   /* a(n-4) when n=6 */
    seq_t a3 = (seq_t)25572;  /* a(n-3) */
    seq_t a4 = (seq_t)471492; /* a(n-2) */
    seq_t a5 = (seq_t)8314020;/* a(n-1) */

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t new_val = 30 * a5 - 237 * a4 + 180 * a3 - 36 * a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = new_val;
    }
    return a5;
}
