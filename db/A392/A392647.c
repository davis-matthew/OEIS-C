/*
 * A392647  a(n) is the number of solutions of the Diophantine equation x*y -
 *          x*z + y*z = n such that x, y, z are in {1,2,...,n}.
 * https://oeis.org/A392647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to x*y - x*z + y*z = n with 1 ≤ x,y,z
 *            ≤ n
 * Range:     u8 n <= 63, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count solutions by iterating over all pairs (x, z) and computing y. */
    seq_t count = 0;
    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t z = 1; z <= n; ++z) {
            idx_t d = x + z;                     /* denominator x+z */
            idx_t num = n + x * z;                /* numerator n + x*z */
            if (num % d == 0) {
                idx_t y = num / d;
                if (y >= 1 && y <= n) {
                    ++count;
                }
            }
        }
    }
    return count;
}
