/*
 * A348131  a(n) is the numerator of the relativistic sum of n velocities of
 *          1/n, in units where the speed of light is 1.
 * https://oeis.org/A348131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(2m-1) = m^(2m-1) - (m-1)^(2m-1) a(2m) = ((2m+1)^(2m) -
 *            (2m-1)^(2m)) / 2 (m is a positive integer)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset starts at 1; this value is unused. */
        return 0;
    }

    if ((n & 1) == 1) {                /* odd n: n = 2m-1 */
        idx_t m = (n + 1) / 2;          /* m = (n+1)/2 */
        seq_t exp   = (seq_t)(2 * m - 1);
        seq_t base1 = (seq_t)m;
        seq_t base2 = (seq_t)(m - 1);
        seq_t pow1 = big_pow(base1, exp);
        seq_t pow2 = big_pow(base2, exp);
        return pow1 - pow2;
    } else {                            /* even n: n = 2m */
        idx_t m = n / 2;                /* m = n/2 */
        seq_t exp   = (seq_t)(2 * m);
        seq_t pow1 = big_pow((seq_t)(2 * m + 1), exp);
        seq_t pow2 = big_pow((seq_t)(2 * m - 1), exp);
        seq_t diff = pow1 - pow2;
        return diff / 2;
    }
}
