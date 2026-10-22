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
 * Helpers:   ensure_cache, gpf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ensure_cache.h"
#include "../utils/is_prime.h"
#include "../utils/gpf.h"

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
    uint64_t g = gpf((uint64_t)n);          /* greatest prime factor */
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
