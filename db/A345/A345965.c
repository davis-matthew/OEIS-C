/*
 * A345965  a(1) = 1; for n>1, a(n) = phi(n) + a(n/p) where p is the least
 *          prime divisor of n.
 * https://oeis.org/A345965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n>1, a(n) = phi(n) + a(n/p) where p is the least
 *            prime divisor of n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   ensure_cache, lpf, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "../utils/phi.h"
#include "../utils/lpf.h"
#include "../utils/ensure_cache.h"

/* Compute the value of the A345965 sequence at position n.  The function is
   memoised internally to allow arbitrary order calls. */
seq_t a(idx_t n) {
    /* The cache is indexed directly by the OEIS index.  All entries are
       initialised to 0, which is a valid sentinel because a(n) >= 1 for
       all n >= 1. */
    static seq_t* cache = NULL;
    static size_t cache_cap = 0;            /* allocated size of cache   */
    static size_t elements_filled = 0;      /* how many slots are zeroed */

    /* Make sure cache can address index n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Zero any new slots that have not yet been initialised. */
    if (elements_filled < cache_cap) {
        memset(&cache[elements_filled], 0,
               (cache_cap - elements_filled) * sizeof(seq_t));
        elements_filled = cache_cap;
    }

    /* Return the cached value if it already exists. */
    if (cache[(size_t)n] != 0)
        return cache[(size_t)n];

    /* Base case: a(1) = 1. */
    if (n == 1) {
        cache[1] = (seq_t)1;
        return cache[1];
    }

    /* Find the least prime divisor p of n. */
    uint64_t p = lpf(n);
    /* Compute the next index n/p. */
    idx_t next = n / (idx_t)p;

    /* Recursively calculate a(next). */
    seq_t sub = a(next);
    /* Euler's totient of n. */
    seq_t ph = phi(n);

    /* a(n) = phi(n) + a(n/p). */
    seq_t result = ph + sub;

    /* Store in cache for future calls. */
    cache[(size_t)n] = result;
    return result;
}
