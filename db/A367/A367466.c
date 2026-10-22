/*
 * A367466  Sum of the final digits of the divisors of n.
 * https://oeis.org/A367466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{d|n} (d mod 10)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the helper function floor_isqrt expects an idx_t argument and
   returns the floor of its square root.  It is defined here instead of
   using the utility header to avoid any linkage problems. */
#include <stdint.h>
#include <math.h>

/* Compute the floor of the square root of an unsigned integer n. */
static inline idx_t floor_isqrt(idx_t n)
{
    /* Use long double to avoid loss of precision for large n. */
    idx_t r = (idx_t)sqrt((long double)n);
    /* Correct possible overestimation due to rounding. */
    while ((r+1)*(r+1) <= n) ++r;
    while (r*r > n)          --r;
    return r;
}

/* Return the sum of the last decimal digit of each divisor of n. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n = 0, but safe to return 0 */
        return 0;

    idx_t limit = floor_isqrt(n);
    seq_t sum = 0;

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* d is a divisor.  Add its last digit. */
            sum += (seq_t)(d % 10);

            /* Pair divisor n/d.  Add only if distinct. */
            idx_t e = n / d;
            if (e != d)
                sum += (seq_t)(e % 10);
        }
    }

    return sum;
}
