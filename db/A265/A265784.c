/*
 * A265784  Numerators of lower primes-only best approximates (POBAs) to
 *          sqrt(5); see Comments.
 * https://oeis.org/A265784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   The numbers a(n) are the numerators p of the lower primes‑only
 *            best * approximates (POBAs) to sqrt(5). For each prime
 *            denominator q we take the * largest prime p ≤ floor( sqrt(5)*q
 *            ); the pair (p,q) is kept if p/q is * larger than all previously
 *            kept fractions
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   ipow_i32, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t prev_q = 0;
    static uint64_t prev_p = 0;  /* store previous numerator in 64‑bit */

    if (n == 0) return 0;              /* safety – the sequence starts at a(1) */

    if (cache_len >= n)
        return cache[n - 1];

    /* start search from the smallest prime denominator if nothing computed yet,
       otherwise from the successor of the last accepted denominator. */
    idx_t candidate = (cache_len == 0) ? 2 : prev_q + 1;

    while (cache_len < n) {
        /* find the next prime denominator q */
        while (!is_prime(candidate))
            ++candidate;
        idx_t q = candidate;

        /* compute m = floor( sqrt(5*q*q) ) and find the largest prime ≤ m */
        seq_t tmp = (seq_t)5 * (seq_t)q * (seq_t)q;
        seq_t m_seq = floor_sqrt(tmp);
        idx_t m = (idx_t)m_seq;        /* m is small enough for idx_t */

        uint64_t p = m;
        while (p > 1 && !is_prime((idx_t)p))
            --p;

        /* accept if first term or if p/q > previous best fraction */
        bool accept = false;
        if (cache_len == 0) {
            accept = true;
        } else {
            seq_t left  = (seq_t)p * (seq_t)prev_q;
            seq_t right = (seq_t)prev_p * (seq_t)q;
            if (left > right)
                accept = true;
        }

        if (accept) {
            ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache),
                            sizeof(seq_t));
            cache[cache_len++] = (seq_t)p;
            prev_q = q;
            prev_p = p;
        }

        /* move to next integer to look for the next prime denominator */
        ++candidate;
    }

    return cache[n - 1];
}
