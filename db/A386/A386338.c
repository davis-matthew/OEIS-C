/*
 * A386338  Primes without {2, 8} as digits.
 * https://oeis.org/A386338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 2860, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_forbidden_digit_uint64, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* By searching for primes that contain no digits 2 or 8 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/has_forbidden_digit_uint64.h"

/* Static cache of found primes */
static seq_t *cache          = NULL; /* stored primes */
static size_t cache_len      = 0;    /* number of filled entries */
static size_t cache_cap      = 0;    /* current allocated size */
static uint64_t next_candidate = 3;   /* next integer to test */

/* Extend the cache until it contains at least n primes (n>=1) */
static void ensure_cache(idx_t n)
{
    if (n == 0)                         /* undefined, safety only */
        return;

    if (cache_len >= (size_t)n)
        return;                         /* already long enough */

    while (cache_len < (size_t)n) {
        uint64_t cand64 = next_candidate;

        /* Skip even numbers > 2 */
        if (cand64 > 2 && (cand64 & 1ULL) == 0) {
            next_candidate = cand64 + 1ULL;
            continue;
        }

        /* Skip numbers containing digits 2 or 8 */
        if (has_forbidden_digit_uint64(cand64)) {
            next_candidate = cand64 + 1ULL;
            continue;
        }

        /* Test primality on a seq_t value */
        seq_t cand = (seq_t)cand64;
        if (isPrimeGeneric(cand)) {
            /* Grow cache if necessary */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap << 1 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_len++] = cand;
        }
        next_candidate = cand64 + 1ULL;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, safety only */

    ensure_cache(n);
    return cache[n - 1];
}
