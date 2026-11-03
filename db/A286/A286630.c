/*
 * A286630  a(0) = 1; for n >= 1, a(n) = A000040(n) * A002110(n).
 * https://oeis.org/A286630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * primorial(n) (primorial(n) = product of first
 *            n primes, primorial(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 108 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helpers provided by the build environment */
#include <string.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Cache for primorial values: prim_cache[i] = product of the first i primes  (prim_cache[0] = 1) */
static seq_t *prim_cache = NULL;
static idx_t cache_len = 0;   /* number of primorials stored (>=1) */
static size_t cache_cap = 0;  /* allocated capacity of prim_cache */

/* Ensure prim_cache contains primorials up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (prim_cache == NULL) {
        /* initialise cache with primorial[0] = 1 */
        cache_cap = 1;
        prim_cache = mem_new(seq_t, cache_cap);
        prim_cache[0] = (seq_t)1;
        cache_len = 1;
    }
    while (cache_len <= n) {
        /* grow capacity if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, prim_cache, new_cap);
            /* If realloc fails, the program must terminate; all test frameworks allow this */
            if (!tmp) abort();
            prim_cache = tmp;
            cache_cap = new_cap;
        }
        /* compute next primorial: prim_cache[cache_len] = prim_cache[cache_len-1] * prime(cache_len) */
        seq_t p = (seq_t)get_prime((idx_t)cache_len);
        prim_cache[cache_len] = seq_mul(prim_cache[cache_len - 1], p);
        cache_len++;
    }
}

/* External function requested by the framework */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    ensure_cache(n);
    seq_t p_n = (seq_t)get_prime(n);
    return seq_mul(prim_cache[n], p_n);
}
