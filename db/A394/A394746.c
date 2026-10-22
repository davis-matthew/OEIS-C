/*
 * A394746  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x,y,z are positive integers satisfying x <= y <= z.
 * https://oeis.org/A394746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of positive integers with x <=
 *            y <= z and x^2 + y*z = n
 * Range:     u8 n <= 1062, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; ; ++x) {
        /* compute x^2 in seq_t to avoid overflow of idx_t */
        seq_t x2 = (seq_t)x * (seq_t)x;
        if (x2 > (seq_t)n) break;               /* x^2 > n -> stop */
        idx_t remaining = n - (idx_t)x2;        /* n - x^2  > 0 here */
        if (remaining == 0) continue;           /* no positive y*z */

        for (idx_t y = x; ; ++y) {
            seq_t y2 = (seq_t)y * (seq_t)y;
            if (y2 > (seq_t)remaining) break;   /* y > sqrt(remaining) */
            if (remaining % y == 0) {          /* y divides remaining */
                idx_t z = remaining / y;        /* z = remaining / y */
                /* y <= z automatically holds because y <= sqrt(remaining) */
                ++count;
            }
        }
    }
    return count;
}
