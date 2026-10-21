/*
 * A284928  Numbers k such that 2k + p is composite for all primes p, q with p
 *          + q = 2k.
 * https://oeis.org/A284928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq_array, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: return the n-th integer k for which
   there is no prime p < 2k with 2k - p and 2k + p both prime.
   The sequence starts with a(1)=0. */

/* required types provided by the build environment */
#include <stdbool.h>
#include <stdlib.h>

/* helper libraries – use the provided utilities */
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/alloc_seq_array.h"

/* cache of computed sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* dynamic prime table */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t prime_limit = 0;

/* grow the cache array to at least 'needed' elements */
static void cache_grow(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* extend the prime table up to 'lim' (inclusive) */
static void ensure_primes(idx_t lim)
{
    if (lim <= prime_limit) return;
    idx_t *tmp = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto(lim, (idx_t **)&tmp, &tmp_cnt);
    free(primes);
    primes = tmp;
    primes_cnt = tmp_cnt;
    prime_limit = lim;
}

/* test whether a given k satisfies the OEIS condition */
static bool good_k(idx_t k)
{
    /* 0 and 1 always satisfy the definition */
    if (k <= 1) return true;

    idx_t two_k = k + k;          /* 2k */
    ensure_primes(two_k - 1);     /* need primes < 2k */

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p >= two_k) break;   /* only primes < 2k */
        idx_t q = two_k - p;     /* candidate for 2k - p */
        if (!is_prime(q)) continue;   /* 2k - p not prime, no violation */
        idx_t test = two_k + p;       /* 2k + p */
        if (is_prime(test)) return false; /* violation found */
    }
    return true;
}

/* public sequence routine */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined only for n >= 1 */

    /* initialise cache if first call */
    if (!cache) {
        cache_cap = 64;
        cache = alloc_seq_array(cache_cap);
        cache_len = 0;
    }

    /* extend cache until we have the n-th term */
    static idx_t candidate = 0;   /* candidate k */
    while (cache_len < (size_t)n) {
        if (good_k(candidate))
            cache[cache_len++] = (seq_t)candidate;
        if (candidate == (idx_t)-1) break;  /* overflow guard */
        ++candidate;
    }

    return cache[n - 1];
}
