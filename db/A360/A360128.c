/*
 * A360128  a(n) = 1 if there are no divisors d>1 of n such that also d+1 is a
 *          divisor of n, otherwise 0.
 * https://oeis.org/A360128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if no divisor d>1 of n satisfies (d+1)|n, else 0
 * Range:     u8 n <= 9697, u16 n <= 9697, u32 n <= 9697, u64 n <= 9697,
 *            u128 n <= 9697, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n=1 has no divisors >1, so it is always 1. */
    if (n <= 1)
        return 1;

    bool ok = true;

    /* Loop over all divisors d of n with d <= sqrt(n).  Use n/d to avoid overflow. */
    for (idx_t d = 2; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* Check the divisor d itself. */
            if ((n % (d + 1)) == 0) {
                ok = false;
                break;
            }

            /* Check the complementary divisor e = n/d, if it is distinct from d. */
            idx_t e = n / d;
            if (e != d && (n % (e + 1)) == 0) {
                ok = false;
                break;
            }
        }
    }

    return ok ? 1 : 0;
}
