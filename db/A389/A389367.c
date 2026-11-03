/*
 * A389367  Semilengths of the Dyck natural numbers.
 * https://oeis.org/A389367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1. For n > 1, a(n) = Σ_{p <= gpf(n), p prime} (
 *            1 + a( val_p(n) ) ), where val_p(n) is the largest k ≥ 0 with
 *            p^k | n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 78 (2026-11-03)
 * Helpers:   is_prime, largest_prime_factor, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/mem.h"

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

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* base cases */
    if (n <= 1) {
        return (seq_t)n;            /* a(0)=0 , a(1)=1 */
    }

    /* make sure the cache can hold up to n */
    size_t old_cap = cache_cap;
    ensure_cache(n, &cache, &cache_cap);
    /* initialise any newly allocated cells to 0 (uncomputed) */
    for (size_t i = old_cap; i < cache_cap; ++i) {
        cache[i] = (seq_t)0;
    }

    /* if already computed, return it */
    if (cache[n] != 0) {
        return cache[n];
    }

    /* compute the term */
    uint64_t g = largest_prime_factor((uint64_t)n);          /* greatest prime factor */
    seq_t sum = 0;

    /* iterate over all primes p <= g */
    for (uint64_t p = 2; p <= g; ++p) {
        if (!is_prime(p)) {
            continue;
        }
        /* compute val_p(n) */
        uint64_t tmp = n;
        unsigned e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        /* add 1 + a(e) to the sum */
        sum += 1 + a((idx_t)e);
    }

    cache[n] = sum;
    return sum;
}
