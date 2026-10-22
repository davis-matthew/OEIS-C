/*
 * A374334  a(n) is the numerator of x(n) = (16*x(n-1) + (120*n^2 - 89*n +
 *          16)/(512*n^4 - 1024*n^3 + 712*n^2 - 206*n + 21)) mod 1, with x(0)
 *          = 0.
 * https://oeis.org/A374334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of the rational number x(n), where x(0) =
 *            0 x(n) = (16 * x(n-1) + (120*n^2 - 89*n + 16) / (512*n^4 -
 *            1024*n^3 + 712*n^2 - 206*n + 21)) mod 1 The fraction x(n) is
 *            reduced to lowest terms after each step
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t num = 0;          /* numerator of current x */
    seq_t den = 1;          /* denominator of current x, always >0 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;

        /* numerator and denominator of the added fraction p/q */
        seq_t num_p = 120 * ii * ii - 89 * ii + 16;
        seq_t den_q = 512 * ii * ii * ii * ii
                   - 1024 * ii * ii * ii
                   + 712 * ii * ii
                   - 206 * ii
                   + 21;

        /* lcm(den, den_q) */
        seq_t g = gcd(den, den_q);
        seq_t lcmDen = (den / g) * den_q;

        /* compute new numerator modulo lcmDen to keep numbers small */
        seq_t mul1 = lcmDen / den;
        seq_t mul2 = lcmDen / den_q;

        /* (16 * num / den) part */
        seq_t part1 = (num * 16) * mul1;
        /* p/q part */
        seq_t part2 = num_p * mul2;

        seq_t new_num = part1 + part2;
        seq_t new_num_mod = new_num % lcmDen;
        seq_t new_den = lcmDen;

        /* reduce the fraction */
        seq_t g2 = gcd(new_num_mod, new_den);
        num = new_num_mod / g2;
        den = new_den / g2;
    }

    return num;
}
