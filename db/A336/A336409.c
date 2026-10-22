/*
 * A336409  Distance from prime(n) to the nearest odd composite that is <
 *          prime(n).
 * https://oeis.org/A336409
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = distance from prime(n) to the nearest odd composite <
 *            prime(n)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* No defined value for n below the offset 5 */
    if (n < 5) return (seq_t)0;

    /* Prime cache */
    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_cnt = 0;

    /* Initialise cache on first call */
    if (primes == NULL) {
        primes_cap = 256;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) return (seq_t)0;      /* allocation failure */
        primes[0] = 2;                     /* first prime */
        primes_cnt = 1;
    }

    /* Grow cache until we have at least n primes */
    while (primes_cnt < (size_t)n) {
        uint64_t candidate = primes[primes_cnt-1] + 1;
        while (!is_prime(candidate)) candidate++;

        if (primes_cnt == primes_cap) {
            primes_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;   /* realloc failure */
            primes = tmp;
        }
        primes[primes_cnt++] = candidate;
    }

    /* n‑th prime (1‑based) */
    uint64_t p = primes[n-1];

    /* Find the largest odd composite < p */
    uint64_t m = p - 1;
    if ((m & 1U) == 0) m--;                 /* make it odd */
    for ( ; m >= 3; m -= 2) {
        if (!is_prime(m)) break;            /* m is composite */
    }
    if (m < 3) m = 1;                       /* safety fallback */

    /* Distance */
    return (seq_t)(p - m);
}
