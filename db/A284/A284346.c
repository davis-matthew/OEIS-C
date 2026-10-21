/*
 * A284346  a(n) is the least positive integer such that n^2 + a(n)^2 and (n +
 *          1)^2 + (a(n) + 1)^2 are primes.
 * https://oeis.org/A284346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least positive integer a such that n^2 + a^2 and
 *            (n+1)^2 + (a+1)^2 are both prime
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 105, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_cache.h"

/* Cache for computed values; index 0 corresponds to a(1). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Return the least positive integer a satisfying the condition. */
seq_t a(idx_t n)
{
    /* The sequence starts at n = 1. */
    if (n == 0)
        return 0;

    /* Make sure the cache can hold the value for this n. */
    if ((idx_t)n > cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* If the value is already cached, return it. */
    if (cache[n - 1] != 0)
        return cache[n - 1];

    /* Search for the smallest a. */
    seq_t cand = 1;                 /* candidate a */
    while (true) {
        /* Calculate n^2 + a^2 and (n+1)^2 + (a+1)^2 in seq_t. */
        seq_t x = ((seq_t)n * (seq_t)n) + (cand * cand);
        seq_t y = ((seq_t)(n + 1) * (seq_t)(n + 1))
               + ((cand + 1) * (cand + 1));

        if (is_prime_big(x) && is_prime_big(y))
            break;                 /* found the desired a */

        ++cand;
    }

    /* Store the result in the cache and return it. */
    cache[n - 1] = cand;
    return cand;
}
