/*
 * A296932  Primes p such that Legendre(-23,p) = 0 or 1.
 * https://oeis.org/A296932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9102 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296932:
 * a(n) = the n-th prime p such that Legendre(-23,p) = 0 or 1.
 * Equivalently, p ≡ 0,1,2,3,4,6,8,9,12,13,16,18 (mod 23). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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
    /* Static cache of the qualifying primes found so far. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_checked = 1;   /* integer that has been tested up to */

    /* If n is out of bounds (0), just return 0.  The OEIS offset is 1. */
    if (n < 1) return 0;

    /* Allowed residues of p modulo 23. */
    static const bool allowed[23] = {
        /* 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 */
         1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0
    };

    /* Grow the cache until we have at least n entries. */
    while (cache_len < n) {
        idx_t candidate = last_checked + 1;
        /* Increment candidate until we find the next qualifying prime. */
        while (true) {
            last_checked = candidate;
            if (!is_prime(candidate)) {          /* not prime -> skip */
                candidate++;
                continue;
            }
            if (allowed[candidate % 23]) {       /* prime and residue matches */
                /* Ensure enough space in the cache */
                ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache),
                                sizeof(seq_t));
                cache[cache_len++] = (seq_t)candidate;
                break;  /* break inner while: new prime added */
            }
            candidate++;  /* move to next integer */
        }
    }

    return cache[n - 1];
}
