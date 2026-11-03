/*
 * A307826  The number of integers r such that all primes above a certain
 *          value have the form primorial(n)*q +- r.
 * https://oeis.org/A307826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A002110(n))/2 for n>1, a(1)=1 * (Equivalently
 *            a(n) = ∏_{k=1..n} (p_k-1) / 2, with p_k the k‑th prime)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 344 (2026-11-03)
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
    if (n == 0)               /* offset is 1; n=0 is outside the sequence */
        return (seq_t)0;

    /* static cache for memoised values a(1)..a(max_computed) */
    static seq_t *cache   = NULL;   /* array of cached a(k) */
    static size_t cache_size = 0;   /* current allocated size */
    static idx_t max_computed = 0;  /* largest n for which cache is valid */
    static seq_t prod = (seq_t)1;   /* ∏_{k=1..max_computed} (p_k-1) */

    /* Ensure we have room for a(n) */
    ensure_cache(n, &cache, &cache_size);

    /* Compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            /* a(1) is defined to be 1; product remains 1 */
            cache[0] = (seq_t)1;
            prod    = (seq_t)1;
        } else {
            uint64_t p = get_prime(i);         /* i‑th prime, 1‑indexed */
            prod = prod * (seq_t)(p - 1);      /* update product of (p_j-1) */
            cache[i - 1] = prod / (seq_t)2;    /* a(i) = prod / 2 */
        }
    }

    max_computed = n;
    return cache[n - 1];
}
