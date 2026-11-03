/*
 * A268878  Breadth-first traversal of a binary tree in which the value at the
 *          n-th node is equal to ParentNode()*prime(n-1).
 * https://oeis.org/A268878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = prime(n-1) * a(floor(n/2)), a(1) = 1 (1‑indexed)
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 95, u64 n <= 671,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 866 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

static seq_t *cache      = NULL;   /* computed values, 1‑based */
static size_t cache_cap  = 0;      /* capacity of cache array */
static idx_t  max_computed = 0;    /* largest index already filled */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not needed for offset 1, but safe */

    if (n > max_computed)
    {
        /* make sure the cache can hold index n (use helper to handle ctor) */
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t k = max_computed + 1; k <= n; ++k)
        {
            if (k == 1)
            {
                cache[1] = (seq_t)1;
            }
            else
            {
                seq_t p = (seq_t)get_prime(k - 1);  /* kth prime, 1‑indexed */
                cache[k] = cache[k / 2] * p;
            }
        }
        max_computed = n;
    }

    return cache[n];
}
