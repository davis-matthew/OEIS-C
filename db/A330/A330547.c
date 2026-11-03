/*
 * A330547  a(1)=2; thereafter a(n) = a(n-1) +
 *          (-1)^(n+1)*(prime(n)-prime(n-1)) (where prime(k) denotes the k-th
 *          prime).
 * https://oeis.org/A330547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=2; a(n)=a(n−1)+(-1)^(n+1)*(prime(n)-prime(n-1)) for n>1
 *            where prime(k) is the k‑th prime
 * Range:     i8 n <= 779, i16 n <= 30000, i32 n <= 30000, i64 n <= 30000,
 *            i128 n <= 30000, big any n
 * Validated: standard, pass, 102 terms through n = 9615 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;

    /* ensure cache big enough for index n (1‑based) */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;          /* indices 0..n */
        if (cache) {
            cache = mem_resize(seq_t, cache, new_cap);
        } else {
            cache = mem_new(seq_t, new_cap);
        }
        cache_cap = new_cap;
    }

    /* initialise first term */
    if (last_computed == 0) {
        cache[1] = (seq_t)2;
        last_computed = 1;
    }

    /* compute any missing values up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            uint32_t p_i   = get_prime(i);        /* ith prime  */
            uint32_t p_prev = get_prime(i - 1);    /* (i-1)th prime */
            seq_t diff = (seq_t)(p_i - p_prev);    /* always >=0 */

            /* sign: (-1)^(i+1): + for odd i, - for even i */
            if ((i & 1U) == 0U) diff = -diff;

            cache[i] = cache[i - 1] + diff;
        }
        last_computed = n;
    }

    return cache[n];
}
