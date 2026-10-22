/*
 * A330557  a(n) = number of primes p <= 2*n+1 with Delta(p) == 2 mod 4, where
 *          Delta(p) = nextprime(p) - p.
 * https://oeis.org/A330557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p <= 2n+1 with (nextprime(p)-p) ≡ 2 (mod
 *            4)
 * Range:     u8 n <= 1447, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* The upper bound for the primes to be examined */
    idx_t limit = n * 2 + 1;

    /* Build all primes up to 'limit' */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Count the required primes */
    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* Find the first prime strictly greater than p */
        uint64_t q = next_prime(p + 1);
        /* Check the gap modulo 4 */
        if (((q - p) & 3ULL) == 2ULL) {
            ++count;
        }
    }

    /* Clean up */
    free(primes);

    return count;
}
