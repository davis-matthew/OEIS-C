/*
 * A358051  Squares k such that phi(k) is a cube.
 * https://oeis.org/A358051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   build_spf, ipow_i32, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A358051
 *
 *  Formula: a(n) = A114076(n)^2
 *  where A114076(n) is the n-th integer k such that φ(k) is a perfect
 *  cube.  The sequence consists of the squares of those k.
 *
 *  The implementation generates k in increasing order, tests whether
 *  φ(k) is a cube using `floor_cube_root`, and stores the corresponding
 *  square in a growing cache.  Calls to a(idx_t) are answered from the
 *  cache, so repeated calls in any order require no re‑search of
 *  earlier terms.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/ipow_i32.h"
#include "../utils/totient_seq.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

static seq_t *cache_vals = NULL;    /* stored sequence values a(k) */
static size_t cache_cap = 0;        /* capacity of cache_vals */
static size_t cache_len = 0;        /* number of values stored */

static idx_t *spf = NULL;           /* smallest prime factor array */
static idx_t spf_limit = 0;         /* current maximum index spf covers */

static idx_t next_candidate = 1;    /* next integer n to test */

/* Ensure spf covers all numbers up to 'limit' */
static void ensure_spf_limit(idx_t limit)
{
    if (spf_limit >= limit)
        return;                      /* already sufficient */

    idx_t *old = spf;
    idx_t new_limit = (spf_limit == 0) ? 1024 : spf_limit << 1;
    while (new_limit < limit)
        new_limit <<= 1;             /* at least double */

    build_spf(new_limit, &spf);
    spf_limit = new_limit;
    if (old)
        free(old);                   /* free previous array */
}

/* Compute φ(n) using the spf table */
static seq_t phi_from_idx(idx_t n)
{
    seq_t phi = (seq_t)n;
    idx_t tmp = n;

    /* Edge case n == 1: loop not entered, phi = 1 */
    while (tmp > 1) {
        idx_t p = spf[tmp];
        while (tmp % p == 0)
            tmp /= p;
        phi = phi - phi / (seq_t)p;  /* φ = φ * (1 - 1/p) */
    }
    return phi;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* out of range guard */

    /* Generate terms until we have at least n cached values */
    while (cache_len < (size_t)n) {
        /* Make sure we can factor the next candidate */
        ensure_spf_limit(next_candidate + 1);

        seq_t phi = phi_from_idx(next_candidate);
        seq_t prod = phi * (seq_t)next_candidate;   /* k * φ(k) */

        seq_t r   = floor_cube_root(prod);
        seq_t r2  = r * r;
        seq_t r3  = r2 * r;

        if (r3 == prod) {            /* φ(k) is a cube */
            seq_t val = (seq_t)next_candidate * (seq_t)next_candidate;

            if (cache_len == cache_cap) {            /* grow cache if needed */
                size_t new_cap = cache_cap ? cache_cap * 2u : 16u;
                cache_vals = seq_realloc(cache_vals, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache_vals[cache_len++] = val;
        }
        ++next_candidate;
    }
    return cache_vals[n - 1];
}
