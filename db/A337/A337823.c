/*
 * A337823  a(n) = prime(n-1) - floor(a(n-1)/2); a(1)=1.
 * https://oeis.org/A337823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n-1) - floor(a(n-1)/2); a(1) = 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1
    a(n) = prime(n-1) - floor(a(n-1)/2)   for n ≥ 2
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
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

/* Compute the n-th term of OEIS A337823.
   The function is memoised: the cache is built on demand and
   can be queried in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms */

    /* Initialise: a(1) = 1 in cache[0] */
    if (cache == NULL) {
        cache = mem_new(seq_t, 1);
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                         /* a(1) */
        cache_size = 1;
    }

    /* Return value if already computed */
    if ((idx_t)cache_size >= n) {
        return cache[n - 1];
    }

    /* Extend cache up to term n */
    size_t old_size = cache_size;
    cache_size = (size_t)n;
    seq_t *new_cache = mem_resize(seq_t, cache, cache_size);
    if (new_cache == NULL) {
        /* allocation failure – propagate failure sentinel */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute missing terms iteratively */
    for (idx_t k = old_size + 1; k <= n; ++k) {
        seq_t prev = cache[k - 2];                    /* a(k‑1) */
        seq_t prime_val = (seq_t)get_prime(k - 1);    /* prime(k‑1) */
        seq_t half_prev = prev / (seq_t)2;            /* floor(a(k‑1)/2) */
        cache[k - 1] = prime_val - half_prev;
    }

    return cache[n - 1];
}
