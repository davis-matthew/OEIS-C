/*
 * A386161  Primes having only {2, 5, 6, 9} as digits.
 * https://oeis.org/A386161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 68, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based computation for OEIS A386161:
   a(n) = the n-th prime whose decimal representation uses only
   the digits 2, 5, 6, and 9.  The function builds an internal cache
   of found primes and continues scanning from the last candidate
   in subsequent calls.  Numbers greater than 5 that can be prime
   must end with 9, so after 5 we increment by 10 to skip even
   and 5-ending candidates.  The search stops as soon as the
   requested index `n` is reached. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* internal cache of primes */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* next decimal candidate to test; the enumeration always"
   starts at 2 and then jumps as explained above */
static uint64_t next_candidate = 2;

/* enlarge cache if necessary */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    if (cache) {
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache     = tmp;
    cache_cap = new_cap;
}

/* true if all decimal digits of x are in {2,5,6,9} */
static inline bool allowed_digits(uint64_t x)
{
    while (x) {
        uint8_t d = x % 10;
        if (d != 2 && d != 5 && d != 6 && d != 9) return false;
        x /= 10;
    }
    return true;
}

/* exported sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n<1 */

    /* enough already computed? */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* produce primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t cur = next_candidate;

        /* handle the two small primes 2 and 5 */
        if (cur <= 5) {
            if (cur == 2 || cur == 5) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = (seq_t)cur;
            }
            /* advance to the first candidate ending in 9 */
            cur = (cur == 5) ? 9 : 5;
            next_candidate = cur;
            continue;
        }

        /* cur >= 9 and ends with 9; test only if digits are allowed */
        if (allowed_digits(cur) && is_prime_big((seq_t)cur)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)cur;
        }
        /* move to the next number ending in 9 */
        cur += 10;
        next_candidate = cur;
    }

    return cache[n - 1];
}
