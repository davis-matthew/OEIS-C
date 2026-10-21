/*
 * A251086  Number of (n+1) X (6+1) 0..2 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A251086
   Empirical recurrence (OEIS entry):
     a(1) = 1782
     a(2) = 4848
     a(3) = 8284
     a(4) = 15780
     a(5) = 27040
     a(6) = 47932
     a(7) = 82898
     a(8) = 147152
   For n > 8:
     a(n) = 6*a(n-1) - 13*a(n-2) + 10*a(n-3) + 5*a(n-4)
            - 14*a(n-5) + 9*a(n-6) - 2*a(n-7)
   The implementation memoizes values up to the largest
   requested n, allowing calls in any order.  It works for both
   plain integer seq_t and an arbitrary‑precision integer class
   (compiled as C++17). */

#include <stdlib.h>
#include <stddef.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* First eight terms (1‑based) */
static const seq_t init[8] = {
    (seq_t)1782,
    (seq_t)4848,
    (seq_t)8284,
    (seq_t)15780,
    (seq_t)27040,
    (seq_t)47932,
    (seq_t)82898,
    (seq_t)147152
};

#if defined(__cplusplus)

/* C++ implementation – use std::vector for storage */
static std::vector<seq_t> cache;

/* Expand the cache up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (cache.empty()) {               /* initialise once           */
        cache.resize(9);                /* indices 0..8, 0 unused    */
        for (size_t i = 1; i <= 8; ++i)
            cache[i] = init[i - 1];
    }
    while (cache.size() <= n) {         /* current size is max index+1 */
        idx_t i = (idx_t)(cache.size());   /* next index to compute   */
        seq_t val = cache[i - 1] * ((seq_t)6)        /* 6*a(n‑1) */
                  - cache[i - 2] * ((seq_t)13)        /* –13*a(n‑2) */
                  + cache[i - 3] * ((seq_t)10)        /* +10*a(n‑3) */
                  + cache[i - 4] * ((seq_t)5)         /* +5*a(n‑4) */
                  - cache[i - 5] * ((seq_t)14)        /* –14*a(n‑5) */
                  + cache[i - 6] * ((seq_t)9)         /* +9*a(n‑6) */
                  - cache[i - 7] * ((seq_t)2);        /* –2*a(n‑7) */
        cache.push_back(val);
    }
}

#else

/* C implementation – use raw pointer and realloc */
static seq_t *cache = NULL;         /* 1‑based indices   */
static size_t cache_size = 0;        /* largest index cached (cached value at cache[i], 0 unused) */
static size_t cache_cap   = 0;        /* allocated capacity */

static void ensure_cache(idx_t n)
{
    if (n == 0) return;                     /* nothing to cache            */

    /* Allocate or grow storage as needed   */
    size_t need = (size_t)(n + 1);          /* include unused index 0     */
    if (cache_cap < need) {
        size_t new_cap = need > cache_cap * 2 ? need : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);      /* abort on allocation failure */
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* Fill in missing entries */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init[i - 1];
        } else {
            seq_t val = cache[i - 1] * ((seq_t)6)        /* 6*a(n‑1) */
                    - cache[i - 2] * ((seq_t)13)        /* –13*a(n‑2) */
                    + cache[i - 3] * ((seq_t)10)        /* +10*a(n‑3) */
                    + cache[i - 4] * ((seq_t)5)         /* +5*a(n‑4) */
                    - cache[i - 5] * ((seq_t)14)        /* –14*a(n‑5) */
                    + cache[i - 6] * ((seq_t)9)         /* +9*a(n‑6) */
                    - cache[i - 7] * ((seq_t)2);        /* –2*a(n‑7) */
            cache[i] = val;
        }
    }
    cache_size = n;
}

#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* the sequence is defined for n ≥ 1 */
    ensure_cache(n);
#if defined(__cplusplus)
    return cache[(size_t)n];
#else
    return cache[n];
#endif
}
