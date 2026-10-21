/*
 * A244778  Prime numbers ending in the prime number 97.
 * https://oeis.org/A244778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime of the form 100*k + 97
 * Range:     u8 n <= 2, u16 n <= 167, u32 n <= 168, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/append_candidate.h" /* for append_candidate */
#include "../utils/is_prime_big.h"     /* for is_prime_big */

static seq_t *cache = NULL;   /* array of primes found so far */
static size_t len   = 0;      /* current number of elements in cache */
static size_t cap   = 0;      /* current capacity of cache */
static idx_t  next_k = 0;     /* next k to test (k=0 corresponds to 97) */

seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n primes */
    while (len < n) {
        /* build candidate number 100*k + 97 */
        seq_t cand = 100 * (seq_t)next_k + 97;
        if (is_prime_big(cand))
            append_candidate(&cache, &len, &cap, cand);
        ++next_k;
    }
    return cache[n - 1];
}
