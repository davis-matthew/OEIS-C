/*
 * A397981  Upper (1/3,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), * with
 *            a(0)=0, a(1)=2, a(2)=5, a(3)=11, a(4)=20
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first five terms directly */
    if (n == 0) return 0;
    if (n == 1) return 2;
    if (n == 2) return 5;
    if (n == 3) return 11;
    if (n == 4) return 20;

    /* iterate using the recurrence.  Only the last five values are
     * required at any time, so the algorithm runs in O(1) memory. */
    seq_t x0 = 0, x1 = 2, x2 = 5, x3 = 11, x4 = 20, xi;

    for (idx_t i = 5; i <= n; ++i) {
        xi = 2 * x4 - x3 + x2 - 2 * x1 + x0;  /* a(i) = 2*a(i-1) - a(i-2) + a(i-3) - 2*a(i-4) + a(i-5) */
        x0 = x1;
        x1 = x2;
        x2 = x3;
        x3 = x4;
        x4 = xi;
    }
    return x4;
}
