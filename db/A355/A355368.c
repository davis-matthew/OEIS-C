/*
 * A355368  Maximal GCD of six positive integers with sum n.
 * https://oeis.org/A355368
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ d | d divides n and 6*d <= n }
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 6 the definition is irrelevant; return 0. */
    if (n < 6)
        return (seq_t)0;

    /* Limit for possible gcd (floor(n/6)). */
    idx_t limit = n / 6;
    idx_t best = 0;

    /* Enumerate divisors up to sqrt(n). */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 <= limit && d1 > best)
                best = d1;
            if (d2 <= limit && d2 > best)
                best = d2;
        }
    }

    return (seq_t)best;
}
