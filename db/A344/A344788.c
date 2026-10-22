/*
 * A344788  a(n) is the number of pairs of primes (q,r) with prime(n) > q > r
 *          such that q | r + prime(n) and r | q + prime(n).
 * https://oeis.org/A344788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) counts the number of pairs of primes (q,r) with
   prime(n) > q > r such that q divides r + prime(n) and r divides q + prime(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* prime(n) */
    uint64_t p = get_prime(n);

    /* List of primes strictly less than p */
    uint64_t *primes = NULL;
    size_t plen = 0;
    if (p > 2) {
        build_primes_upto(p - 1, &primes, &plen);
    }

    seq_t count = 0;

    for (size_t i = 0; i < plen; ++i) {
        uint64_t q = primes[i];

        /* Candidate r that satisfies q | r + p */
        uint64_t r = q - (p % q);

        /* r must be a proper prime < q */
        if (r >= q || r <= 1) continue;
        if (!is_prime_small(r)) continue;

        /* Verify the reciprocal divisibility condition r | q + p */
        if ((q + p) % r != 0) continue;

        ++count;
    }

    free(primes);
    return count;
}
