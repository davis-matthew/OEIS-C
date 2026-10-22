/*
 * A364847  Number of conjugacy classes in the group SL(2, Z_n), up to
 *          conjugacy in GL(2, Z_n).
 * https://oeis.org/A364847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d * A060594(n/d)
 * Range:     u8 n <= 59, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   A060594_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A060594(m) is the number of solutions of x^2 = 1 (mod m) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/A060594_value.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisors d using i <= n/i to avoid overflow */
    for (idx_t i = 1U; i <= n / i; ++i)
    {
        if (n % i == 0U)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            sum += (seq_t)d1 * A060594_value(d2);
            if (d1 != d2)
                sum += (seq_t)d2 * A060594_value(d1);
        }
    }
    return sum;
}
