/*
 * A244736  Numbers k such that (prime(k) mod 5) is even.
 * https://oeis.org/A244736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 961 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A244736: a(n) is the n-th positive integer k such that prime(k) mod 5 is even
   (i.e. the residue 0, 2 or 4).  The function builds the sequence on demand
   and caches the already found terms so that calls in arbitrary order are
   fast. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    /* n is always ≥ 1 by the definition of the sequence. */
    static seq_t *cache = NULL;           /* array of found indices k     */
    static size_t cache_len = 0;          /* number of stored elements    */
    static size_t cache_cap = 0;          /* capacity of cache            */
    static idx_t last_k = 0;              /* last tested prime index      */

    /* Extend the cache until it contains at least n elements. */
    if (cache_len < (size_t)n) {
        size_t needed = (size_t)n;
        if (needed > cache_cap) {
            /* Grow the cache geometrically. */
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            while (new_cap < needed) new_cap *= 2;
            ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        }

        /* Generate further terms until we have the required number. */
        while (cache_len < (size_t)n) {
            ++last_k;                                /* next prime index   */
            uint32_t p = get_prime(last_k);          /* prime(last_k)      */
            uint32_t r = p % 5;                      /* residue modulo 5  */
            if ((r & 1) == 0) {                      /* residue is even   */
                cache[cache_len++] = (seq_t)last_k;
            }
        }
    }

    return cache[n - 1];   /* 1‑based indexing for the sequence */
}
