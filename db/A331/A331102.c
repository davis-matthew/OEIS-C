/*
 * A331102  a(n) is the greatest prime number of the form n mod (2^k) for some
 *          k > 0, or 0 if no such prime number exists.
 * https://oeis.org/A331102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = the largest prime among the binary suffixes of n (i.e.
 *            the numbers n mod 2^k for k > 0), or 0 if no such prime exists
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t nn = n;

    /* Determine the bit length of n. */
    uint64_t bitlen = 0;
    uint64_t temp = nn;
    while (temp) {
        temp >>= 1;
        ++bitlen;
    }

    uint64_t maxprime = 0;
    /* Iterate over all possible suffix lengths k = 1 .. bitlen. */
    for (uint64_t k = 1; k <= bitlen; ++k) {
        uint64_t mask;
        if (k == 64)
            mask = ~0ULL;                 /* 2^64 - 1 */
        else
            mask = (1ULL << k) - 1;       /* (1<<k)-1 */
        uint64_t r = nn & mask;           /* n mod 2^k */
        if (r > 1 && is_prime((idx_t)r)) {
            if (r > maxprime)
                maxprime = r;
        }
    }
    return (seq_t)maxprime;
}
