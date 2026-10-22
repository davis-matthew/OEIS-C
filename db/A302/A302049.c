/*
 * A302049  a(n) = 1 if n = prime(k)*prime(1+k) for some k, otherwise 0.
 * https://oeis.org/A302049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = prime(k)*prime(k+1) for some k, else 0
 * Range:     u8 n <= 9738, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Return the smallest prime greater than start */
static idx_t next_prime_idx(idx_t start)
{
    idx_t s = start + 1;
    while (!is_prime(s))
        ++s;
    return s;
}

seq_t a(idx_t n)
{
    /* Smallest product of two successive primes is 2*3 = 6 */
    if (n < 6)
        return 0;

    /* iterate over possible first prime factors */
    for (idx_t p = 2; p <= n / p; ++p)   /* safe up to 2^64 */
    {
        if (n % p != 0)
            continue;
        if (!is_prime(p))
            continue;

        idx_t q = n / p;
        if (q <= p)          /* need strictly larger prime */
            continue;
        if (!is_prime(q))
            continue;

        if (next_prime_idx(p) == q)
            return 1;
    }
    return 0;
}
