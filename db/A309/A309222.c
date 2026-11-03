/*
 * A309222  a(0) = 6; thereafter a(n) = a(n-1) + prime(n) if prime(n) >
 *          a(n-1), otherwise a(n) = a(n-1) - prime(n).
 * https://oeis.org/A309222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=6; for n≥1, a(n)=a(n−1)+prime(n) if prime(n)>a(n−1) else
 *            a(n−1)−prime(n)
 * Range:     u8 n <= 34, u16 n <= 3593, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9494 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    /* static cache for consecutive calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache with a(0) */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)6;
        cache_len = 1;
    }

    if (n == 0) {
        return cache[0];
    }

    /* grow the cache if necessary */
    if (n + 1 > cache_len) {
        size_t target = n + 1;
        if (target > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < target) {
                new_cap <<= 1;
            }
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) {
                /* allocation failure – return 0 as a safe fallback */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* i is 1‑based index for prime(n) */
            uint32_t p32 = get_prime(i);          /* get nth prime, 32‑bit */
            seq_t p = (seq_t)p32;                 /* promote to seq_t */
            if (p > cache[i-1]) {
                cache[i] = cache[i-1] + p;
            } else {
                cache[i] = cache[i-1] - p;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
