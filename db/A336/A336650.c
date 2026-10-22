/*
 * A336650  a(n) = p^e, where p is the smallest odd prime factor of n, and e
 *          is its exponent, with a(n) = 1 when n is a power of two.
 * https://oeis.org/A336650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^e where p is the smallest odd prime factor of n; a(n) =
 *            1 if n is a power of two
 * Range:     u8 n <= 256, u16 n <= 9874, u32 n <= 9874, u64 n <= 9874,
 *            u128 n <= 9874, big any n
 * Helpers:   _smallest_prime_factor, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Power of two check */
    if ((n & (n - 1)) == 0)
        return (seq_t)1;

    uint64_t n64 = (uint64_t)n;
    uint64_t temp = n64;

    /* Remove all factors of 2 to isolate the smallest odd prime factor */
    while ((temp & 1) == 0)   /* temp % 2 == 0 */
        temp >>= 1;           /* temp /= 2 */

    if (temp == 1)            /* n was a power of two */
        return (seq_t)1;

    /* Find the smallest odd prime factor of the reduced number */
    uint64_t p = _smallest_prime_factor(temp);

    /* Compute the exponent of p in the original n */
    idx_t exp = 0;
    uint64_t t = n64;
    while (t % p == 0) {
        exp++;
        t /= p;
    }

    /* Return p^exp */
    return pow_uint(p, exp);
}
