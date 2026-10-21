/*
 * A246933  a(n) = prime(16*n).
 * https://oeis.org/A246933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(16*n)
 * Range:     u8 n <= 3, u16 n <= 408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/_prime_limit_for_idx.h"

/* Static storage for the prime list */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Ensure that prime_list contains at least target primes (1‑based). */
static void ensure_prime_count(idx_t target)
{
    if (target <= prime_count)
        return;

    /* free the previous list if any */
    if (prime_list != NULL)
        free(prime_list);

    /* Allocate a new list with enough primes.
       _prime_limit_for_idx guarantees that a limit returned yields at least
       "target" primes. */
    idx_t limit = _prime_limit_for_idx(target);
    build_primes_upto(limit, &prime_list, &prime_count);

    /* If the estimate was too low, keep expanding until we have enough. */
    while (prime_count < target) {
        limit = _prime_limit_for_idx(target);
        free(prime_list);
        build_primes_upto(limit, &prime_list, &prime_count);
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; the required prime index is 16*n. */
    idx_t idx = n << 4;          /* 16 * n, safe for uint64_t */
    ensure_prime_count(idx);     /* make sure we have idx primes */

    /* prime_list[0] is the first prime, so prime_list[idx-1] is the idx‑th */
    return (seq_t)prime_list[idx - 1];
}
