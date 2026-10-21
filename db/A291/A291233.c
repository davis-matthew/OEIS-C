/*
 * A291233  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S - S^2 - S^3.
 * https://oeis.org/A291233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A291233 -- p-INVERT of (0,1,0,1,0,1,...) with p(S)=1-S-S^2-S^3
   Recurrence (for n ≥ 7):
        a(n) = a(n‑1) + 4·a(n‑2) – a(n‑3) – 4·a(n‑4) + a(n‑5) + a(n‑6)
   Initial terms (a(0) … a(6)):
        1, 2, 5, 11, 26, 58, 134
*/

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* initial seven terms */
static const seq_t init[7] = {
    (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)11,
    (seq_t)26, (seq_t)58, (seq_t)134
};

/* caching mechanisms */
#ifdef __cplusplus
static std::vector<seq_t> cache;   /* vector for C++ builds */
static size_t max_computed = 0;    /* highest computed index */
#else
static seq_t *cache = NULL;        /* array for C builds */
static size_t cache_cap = 0;
static size_t max_computed = 0;    /* highest computed index */
#endif

/* Ensure the cache can hold terms up to index n */
static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    /* C++: use std::vector */
    if (cache.empty()) {
        size_t need = (size_t)n + 1;
        if (need < 7) need = 7;
        cache.resize(need);
        /* initialise first seven terms */
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        max_computed = 6;
    } else if (cache.size() <= (size_t)n) {
        cache.resize((size_t)n + 1);
    }
#else
    /* C: use plain malloc/realloc */
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
        if (need < 8) need = 8;          /* minimum 8 entries */
        cache     = (seq_t*)malloc(need * sizeof(seq_t));
        cache_cap = need;
        /* initialise first seven entries */
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        max_computed = 6;
    } else if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap <<= 1;   /* grow to power of two */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache     = tmp;
        cache_cap = new_cap;
    }
    /* Newly allocated entries beyond max_computed remain uninitialised but
       will be overwritten when computed. */
#endif
}

/* Compute all terms up to n using the recurrence */
static void compute_to(idx_t n)
{
    for (size_t k = max_computed + 1; k <= (size_t)n; ++k) {
        seq_t a1 = cache[k-1];
        seq_t a2 = cache[k-2];
        seq_t a3 = cache[k-3];
        seq_t a4 = cache[k-4];
        seq_t a5 = cache[k-5];
        seq_t a6 = cache[k-6];

        /* a(k) = a1 + 4*a2 - a3 - 4*a4 + a5 + a6   (all seq_t operations) */
        seq_t val = a1
                    + a2 * (seq_t)4
                    + a5
                    + a6
                    - a3
                    - a4 * (seq_t)4;

        cache[k] = val;
    }
    max_computed = (size_t)n;
}

/* public function satisfying the required signature */
seq_t a(idx_t n)
{
    if (n < 7) return init[(size_t)n];

    ensure_cache(n);
    if ((size_t)n > max_computed) compute_to(n);
    return cache[(size_t)n];
}
