/*
 * A292779  Interpret the values of the Moebius function mu(k) for k = n to 1
 *          as a balanced ternary number.
 * https://oeis.org/A292779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, base
 * Formula:   a(n) = Sum_{k = 1 .. n} mu(k) * 3^(k-1)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 40, i128 n <= 81,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but handle 0 safely */

    seq_t sum   = 0;
    seq_t power = 1;                  /* 3^0 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t mu = mobius(k);         /* mu(k) is -1, 0, or 1 in seq_t form */
        if (mu)                       /* skip addition when mu == 0 */
            sum += mu * power;        /* add or subtract the current power of 3 */

        power = power * 3;            /* prepare 3^k for next iteration */
    }

    return sum;
}
