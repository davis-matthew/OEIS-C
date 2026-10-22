/*
 * A393712  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x, y, z are positive integers satisfying x < y <= z.
 * https://oeis.org/A393712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of positive integers with x < y
 *            <= z and x^2 + y*z = n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t count = 0;
    /* iterate over possible x where x^2 < n   */
    for (idx_t x = 1; x <= (n - 1) / x; ++x)
    {
        idx_t t = n - x * x;          /* remaining product y*z */
        /* count divisor pairs (d, t/d) with d <= t/d and d > x */
        for (idx_t d = 1; d <= t / d; ++d)
        {
            if (t % d == 0 && d > x)
            {
                ++count;
            }
        }
    }
    return count;
}
