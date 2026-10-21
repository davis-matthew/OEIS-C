/*
 * A276505  Number of rational numbers > 1 whose numerator in reduced form
 *          equals n and that can be written as a continued fraction with
 *          exactly three partial quotients.
 * https://oeis.org/A276505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{(x,y,z) positive integers | n = x*y*z + x + z}
 *            Equivalent to counting the positive integer solutions of the
 *            Diophantine equation n = xyz + x + z
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* iterate over x and z; for each pair check whether (n - x - z) is
       a positive multiple of (x*z). */
    for (idx_t x = 1; x < n; ++x) {
        for (idx_t z = 1; x + z < n; ++z) {
            idx_t lhs  = n - (x + z);      /* n - x - z > 0 because x+z<n */
            idx_t prod = x * z;            /* positive integer product    */
            if (lhs % prod == 0) {
                /* If (n - x - z) is divisible by (x*z), then
                   y = (n - x - z) / (x*z) is a positive integer. */
                ++count;
            }
        }
    }
    return count;
}
