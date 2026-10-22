/*
 * A329760  Primes without {2, 7} as digits.
 * https://oeis.org/A329760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A329760 = primes whose decimal representation contains neither 2
 *            nor 7
 * Range:     u8 n <= 26, u16 n <= 2158, u32 n <= 9991, u64 n <= 9991,
 *            u128 n <= 9991, big any n
 * Helpers:   bad_digit_present, ensure_cache, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_cache.h"
#include "../utils/bad_digit_present.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_capacity = 0;    /* allocated capacity of cache */
static size_t cache_size = 0;        /* number of entries currently stored */

seq_t a(idx_t n)
{
    if (n == 0)                 /* the sequence is 1‑based; n==0 is unused */
        return 0;

    /* Ensure the cache has room for at least n elements. */
    ensure_cache(n, &cache, &cache_capacity);

    /* Generate terms until we have n elements in the cache. */
    while (cache_size < (size_t)n) {
        /* Start searching from the next integer after the last stored prime. */
        uint64_t cand = (cache_size == 0) ? 2
                                          : (uint64_t)(cache[cache_size - 1]) + 1;

        /* Find the next prime that avoids digits 2 and 7. */
        for (;;) {
            if (bad_digit_present(cand)) {
                ++cand;
                continue;
            }
            if (is_prime((idx_t)cand)) {
                cache[cache_size++] = (seq_t)cand;
                break;
            }
            ++cand;
        }
    }

    return cache[n - 1];
}
