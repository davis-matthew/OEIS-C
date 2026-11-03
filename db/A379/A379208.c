/*
 * A379208  Numbers k such that prime(k) and prime(k) + 9 are anagrams.
 * https://oeis.org/A379208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 2279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9045 (2026-11-01)
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that prime(k) and prime(k)+9 are anagrams. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
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

static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_len = 0;
static idx_t last_checked_k = 0u;

/* Extend the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (seq_cache_len < (size_t)n)
    {
        /* Check the next prime index. */
        last_checked_k++;
        uint32_t p = get_prime(last_checked_k);
        seq_t q = (seq_t)p + (seq_t)9;

        unsigned char cnt_p[10] = {0};
        unsigned char cnt_q[10] = {0};
        digit_counts((seq_t)p, cnt_p);
        digit_counts(q, cnt_q);

        bool same = true;
        for (int d = 0; d < 10; d++)
        {
            if (cnt_p[d] != cnt_q[d])
            {
                same = false;
                break;
            }
        }

        if (same)
        {
            /* Ensure there is room in seq_cache. */
            if (seq_cache_len >= seq_cache_cap)
            {
                size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 8u;
                seq_t *tmp = seq_realloc(seq_cache, seq_cache_cap, new_cap);
                seq_cache = tmp;
                seq_cache_cap = new_cap;
            }
            seq_cache[seq_cache_len++] = (seq_t)last_checked_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but harmless */

    ensure_cache(n);
    return seq_cache[n - 1];
}
