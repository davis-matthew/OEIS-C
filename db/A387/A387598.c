/*
 * A387598  a(n) is the number of 3 element sets of distinct integer sided
 *          strict rectangles that fill an n X n square.
 * https://oeis.org/A387598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n - 1)/2)*(n - 3) + round((n - 3)^2/12)
 * Range:     u8 n <= 23, u16 n <= 337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (N < 3)
        return 0;          /* a(1) = a(2) = a(3) = 0 */

    seq_t t1 = ((N - 1) / 2) * (N - 3);   /* floor((n-1)/2)*(n-3) */
    seq_t tmp = N - 3;
    seq_t t2 = ((tmp * tmp) + 6) / 12;    /* round((n-3)^2/12) == (p+6)/12 */
    return t1 + t2;
}
