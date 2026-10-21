/*
 * A256585  Primes of the form 3n^2 + 39n + 37.
 * https://oeis.org/A256585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   primes of the form 3*n^2 + 39*n + 37. Return the n-th such prime
 *            (a(1) = 37). The function lazily generates values and caches
 *            them
 * Range:     u8 n <= 5, u16 n <= 95, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* seq_t and idx_t are defined by the build. */
static seq_t *cache = NULL;          /* cached prime values */
static size_t cache_len = 0;         /* number of cached primes */
static size_t cache_cap = 0;         /* capacity of the cache array */
static idx_t next_m = 0;             /* next polynomial index to test */

seq_t a(idx_t n) {
    /* Since the sequence is 1‑based, return cache[n‑1] after ensuring we
       have at least n terms cached.  Ignore behavior for n < 1 (undefined). */
    if (n == 0) return 0;

    while (cache_len < n) {
        /* Compute the polynomial value for current m. */
        seq_t m_seq = (seq_t)next_m;
        seq_t val = (3 * m_seq * m_seq) + (39 * m_seq) + 37;

        /* Accept the value only if it is prime. */
        if (is_prime_big(val)) {
            append_candidate(&cache, &cache_len, &cache_cap, val);
        }

        ++next_m;
    }

    return cache[(size_t)(n - 1)];
}
