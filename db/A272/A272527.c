/*
 * A272527  Numbers k such that prime(k) - 2 is the average of four
 *          consecutive odd squares.
 * https://oeis.org/A272527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   ipow_i32, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A272527: a(n) is the index k for which prime(k) = 4*m^2+8*m+11 (m≥1).  
 * The function generates primes incrementally and stores qualifying
 * indices in a static cache.  It returns the nth element of that cache.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
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

/* static cache of qualifying prime indices */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* state for prime generation */
static idx_t cur_candidate = 2;   /* integer value to test for primality */
static idx_t prime_index   = 0;   /* count of primes found so far */

/* generate primes until cache holds at least 'need' elements */
static void a_generate_to(idx_t need)
{
    while (cache_size < need) {
        /* find next prime */
        while (!is_prime(cur_candidate)) {
            cur_candidate++;
            if (cur_candidate > 2 && (cur_candidate & 1) == 0)
                cur_candidate++;
        }

        /* prime found at cur_candidate */
        prime_index++;

        seq_t p = (seq_t)cur_candidate;

        /* only primes ≥ 23 can satisfy p = 4m^2+8m+11 (m≥1) */
        if (p >= 23) {
            /* (p-11) must be divisible by 4 */
            if (((p - 11) & 3) == 0) {
                seq_t s = (p - 11) / 4;   /* s = m^2 + 2m          */
                seq_t t = s + 1;          /* t = (m+1)^2           */
                seq_t r = floor_sqrt(t);
                if (r * r == t) {         /* t is a perfect square */
                    ensure_capacity(cache_size + 1, &cache_cap,
                                    OEISC_VPP(&cache), sizeof(seq_t));
                    cache[cache_size++] = prime_index;
                }
            }
        }

        /* advance to the next candidate */
        cur_candidate++;
        if (cur_candidate > 2 && (cur_candidate & 1) == 0)
            cur_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (cache_size < n) a_generate_to(n);
    return cache[n - 1];
}
