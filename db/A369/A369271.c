/*
 * A369271  The smallest positive number that has not yet appeared that is a
 *          multiple of the smallest prime that does not divide n.
 * https://oeis.org/A369271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A369271
   For n ≥ 1 let p(n) be the smallest prime that does not divide n.
   a(n) is the smallest multiple of p(n) that has not appeared among
   a(1), … , a(n−1). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0 */

    /* Static state – cached sequence terms and data structures */
    static seq_t   *cache   = NULL;  /* array of terms, 0‑based */
    static size_t  cache_cap = 0;    /* allocated capacity of cache */
    static size_t  cache_len = 0;    /* terms already computed (0…len‑1) */

    static bool   *used    = NULL;   /* used[v] = true if value v already seen */
    static size_t  used_cap = 0;     /* capacity of used array */

    static uint64_t *primes   = NULL; /* list of primes for p(n) lookup */
    static size_t   primes_cnt = 0;   /* number of primes in list */

    /* Build list of primes once – 200 000 is ample for n ≤ 10 000 */
    if (primes == NULL) {
        build_primes_upto(200000, &primes, &primes_cnt);
    }

    /* Ensure cache can hold n terms (cache_len is size_t but n is idx_t) */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the missing terms up to n */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        /* 1. Find smallest prime that does NOT divide i */
        uint64_t p = 2;
        for (size_t j = 0; j < primes_cnt; ++j) {
            p = primes[j];
            if (i % p != 0) break;
        }

        /* 2. Find smallest unused multiple of p */
        seq_t cand = (seq_t)p;
        for (;;) {
            /* The sequence values are modest for moderate n, so we can
               safely cast to idx_t (or size_t) when indexing the used array. */
            idx_t idx = (idx_t)cand;

            /* Grow used array if necessary */
            if ((size_t)idx >= used_cap) {
                size_t old_used_cap = used_cap;
                size_t new_used_cap = used_cap ? used_cap * 2 : 128;
                while ((size_t)idx >= new_used_cap) new_used_cap <<= 1;
                bool *tmp = (bool *)realloc(used, new_used_cap * sizeof(bool));
                if (!tmp && new_used_cap > 0) abort();
                /* initialise the new part to false */
                if (new_used_cap > old_used_cap)
                    memset(tmp + old_used_cap, 0,
                           (new_used_cap - old_used_cap) * sizeof(bool));
                used = tmp;
                used_cap = new_used_cap;
            }

            if (!used[(size_t)idx]) {
                used[(size_t)idx] = true;
                cache[i - 1] = cand;
                break;
            }
            cand += (seq_t)p;   /* try next multiple of p */
        }
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
