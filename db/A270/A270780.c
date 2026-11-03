/*
 * A270780  Let p_i = the i-th prime. a(i) is the smallest n>1 such that p_i
 *          divides n!-1.
 * https://oeis.org/A270780
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 76 terms through n = 4915 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A270780: a(n) = smallest m>1 such that the n-th prime p divides m! – 1,
   offset 3.  A search for the minimal m is performed via factorial
   computation modulo p. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* sequence is undefined for n<3; return 0 as a sentinel */
    if (n < 3) return (seq_t)0;

    /* static cache of previously computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;

    /* make sure we have space for the requested index */
    ensure_cache(n, &cache, &cache_cap);

    /* use 0 as “uncomputed” sentinel (minimal value is 2) */
    if (cache[n] != (seq_t)0) return cache[n];

    /* fetch the nth prime (primes are 1‑based) */
    uint32_t p = get_prime(n);       /* p ≥ 5 for n ≥ 3 */

    /* compute m! modulo p until it becomes 1 (m>1) */
    seq_t cur = (seq_t)(2 % p);      /* 2! mod p */

    if (cur == (seq_t)1) {
        cache[n] = (seq_t)2;
        return cache[n];
    }

    /* iterate m = 3 … p‑2 (by Wilson’s theorem a solution always exists) */
    for (idx_t m = 3; m <= (idx_t)(p - 2); ++m) {
        uint64_t prod = (uint64_t)cur * (uint64_t)m;   /* avoid overflow */
        cur = (seq_t)(prod % (uint64_t)p);

        if (cur == (seq_t)1) {
            cache[n] = (seq_t)m;
            return cache[n];
        }
    }

    /* fallback: the guaranteed solution is p‑2 */
    cache[n] = (seq_t)(p - 2);
    return cache[n];
}
