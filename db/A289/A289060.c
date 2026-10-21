/*
 * A289060  a(n) = 3*a(n-1) - 3*a(n-2) + *a(n-3) for n >= 8, where a(0) = 2,
 *          a(1) = 4, a(2) = 7, a(3) = 11, a(4) = 17, a(5) = 25, a(6) = 36,
 *          a(7) = 51.
 * https://oeis.org/A289060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30 - 11*n + 2*n^2 for n > 4 a(0) = 2, a(1) = 4, a(2) = 7,
 *            a(3) = 11, a(4) = 17
 * Range:     u8 n <= 13, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[5] = {2, 4, 7, 11, 17};
    if (n <= 4)
        return init[n];
    seq_t nn = (seq_t)n;
    seq_t res = 2 * nn * nn;   /* 2*n^2 */
    res -= 11 * nn;            /* subtract 11*n */
    res += 30;                 /* add 30 */
    return res;
}
