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
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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
        mem_set(&cache[elements_filled], 0, (cache_cap - elements_filled) * sizeof(seq_t));
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
    seq_t ph = totient(n);

    /* a(n) = phi(n) + a(n/p). */
    seq_t result = ph + sub;

    /* Store in cache for future calls. */
    cache[(size_t)n] = result;
    return result;
}
