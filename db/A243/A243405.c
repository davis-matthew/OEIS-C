/*
 * A243405  Minimum among the numbers p^(n/p), where p is a prime factor of n.
 * https://oeis.org/A243405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum of p^(n/p) over all prime divisors p of n. (If n
 *            = 1, a(1) = 1.) The computation uses the largest prime factor
 *            (gpf) to avoid iterating over all divisors
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 242, big any n
 * Helpers:   big_pow, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t g = gpf(n);                 /* largest prime factor of n   */

    if (g > 3)                            /* prime factor > 3 dominates   */
    {
        idx_t exp = n / g;                /* exponent is n / g            */
        return big_pow((seq_t)g, (seq_t)exp);
    }

    /* g <= 3 */
    if ((n & 1) == 0)                     /* n is even → 2^(n/2) is smallest */
    {
        idx_t exp = n / 2;
        return big_pow((seq_t)2, (seq_t)exp);
    }
    /* n is odd and g <= 3 → g must be 3 */
    idx_t exp = n / 3;
    return big_pow((seq_t)3, (seq_t)exp);
}
