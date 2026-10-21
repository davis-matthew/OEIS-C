/*
 * A295666  a(n) = Product_{d|n, gcd(d,n/d) is prime} gcd(d,n/d).
 * https://oeis.org/A295666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n; gcd(d,n/d) is prime} gcd(d,n/d)
 * Range:     u8 n <= 107, u16 n <= 539, u32 n <= 3779, u64 n <= 9916,
 *            u128 n <= 9916, big any n
 * Helpers:   gcd, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; safeguard for n == 0. */
    if (n == 0)
        return 1;

    seq_t result = 1;

    /* Iterate over each divisor d1 of n, using the fact that if d1 ≤ n/d1
       then d2 = n / d1 is also a divisor. This ensures every divisor is
       examined exactly once for each of the two possible positions in the
       pair (d1, d2). */
    for (idx_t d1 = 1; d1 <= n / d1; ++d1)
    {
        if (n % d1 != 0)          /* not a divisor – skip */
            continue;

        idx_t d2 = n / d1;        /* complementary divisor */
        seq_t g = gcd((seq_t)d1, (seq_t)d2);

        if (is_prime_small(g))    /* g > 1 and prime */
        {
            result *= g;          /* for divisor d1 */
            if (d1 != d2)         /* multiplier for d2 if distinct */
                result *= g;
        }
    }

    return result;
}
