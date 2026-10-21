/*
 * A250953  Number of (n+1)X(5+1) 0..1 arrays with no 2X2 subblock having its
 *          maximum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A250953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 57*a(n‑1) - 516*a(n‑2) + 637*a(n‑3) + 2799*a(n‑4) -
 *            3639*a(n‑5) - 100*a(n‑6) + 429*a(n‑7); Initial values:
 *            a(1)=2911, a(2)=134456, a(3)=6204344, a(4)=286298344,
 *            a(5)=13211127617, a(6)=609622239024, a(7)=28130774228952. The
 *            function memoizes results so that calls in any order are fast
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* In C++, use new/delete for allocation; in C use malloc/free. */
#ifdef __cplusplus
#define CACHE_ALLOC(cap) new seq_t[(cap)]
#define CACHE_FREE(ptr)  delete [] (ptr)
#else
#define CACHE_ALLOC(cap) ((seq_t*)malloc((cap) * sizeof(seq_t)))
#define CACHE_FREE(ptr)  free((ptr))
#endif

static seq_t *cache     = NULL;   /* 1‑based array: cache[i‑1] = a(i) */
static size_t cache_len  = 0;     /* number of terms stored (>=0) */
static size_t cache_cap  = 0;     /* allocated capacity */

/* Ensure cache can hold at least 'need' terms. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_cache = CACHE_ALLOC(new_cap);
    if (!new_cache) return; /* allocation failure: abort silently */

    /* Copy existing terms if reallocating. */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        CACHE_FREE(cache);
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Initialize the first seven values (only once). */
static void init_cache(void)
{
    if (cache_len != 0) return; /* already initialized */

    cache[0] = 2911ull;
    cache[1] = 134456ull;
    cache[2] = 6204344ull;
    cache[3] = 286298344ull;
    cache[4] = 13211127617ull;
    cache[5] = 609622239024ull;
    cache[6] = 28130774228952ull;
    cache_len = 7;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, return 0 safely */

    /* Allocate/capacity check. */
    ensure_capacity((size_t)n);
    init_cache();

    /* If requested term already computed, return it. */
    if (n <= cache_len) return cache[(size_t)(n - 1)];

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* i >= 8 is guaranteed here because cache_len starts at 7. */
        size_t idx = i - 1;
        seq_t v1 = cache[idx - 1];
        seq_t v2 = cache[idx - 2];
        seq_t v3 = cache[idx - 3];
        seq_t v4 = cache[idx - 4];
        seq_t v5 = cache[idx - 5];
        seq_t v6 = cache[idx - 6];
        seq_t v7 = cache[idx - 7];
        seq_t val = v1 * 57
                  - v2 * 516
                  + v3 * 637
                  + v4 * 2799
                  - v5 * 3639
                  - v6 * 100
                  + v7 * 429;
        cache[idx] = val;
    }

    cache_len = (size_t)n;
    return cache[(size_t)(n - 1)];
}
