/*
 * A369749  a(n) = Sum_{p|n, p prime} p * prime(n/p).
 * https://oeis.org/A369749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p * prime(n/p)
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"

/* Compute a(n) for any n >= 1.
   prime(k) is obtained from get_prime, a 1‑based prime index. */
seq_t a(idx_t n)
{
    if (n < 2)          /* a(1) = 0 by definition */
        return (seq_t)0;

    seq_t sum = 0;
    uint64_t m = (uint64_t)n;

    /* handle factor 2 separately */
    if ((m & 1ULL) == 0ULL) {
        uint64_t p = 2ULL;
        idx_t idx = n / p;                /* n/p is integer */
        sum += (seq_t)p * (seq_t)get_prime(idx);
        while ((m & 1ULL) == 0ULL)
            m >>= 1;                      /* divide out all 2's */
    }

    /* odd factors */
    for (uint64_t d = 3ULL; d * d <= m; d += 2ULL) {
        if (m % d == 0ULL) {
            uint64_t p = d;
            idx_t idx = n / p;
            sum += (seq_t)p * (seq_t)get_prime(idx);
            while (m % d == 0ULL)
                m /= d;                  /* divide out all copies of d */
        }
    }

    /* if a prime factor larger than sqrt remains */
    if (m > 1ULL) {
        uint64_t p = m;
        idx_t idx = n / p;
        sum += (seq_t)p * (seq_t)get_prime(idx);
    }

    return sum;
}
