/*
 * A283852  Number of n X 3 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence a(n) with linear recurrence a(n) = 7*a(n-1) - 2*a(n-2)
 *            + 19*a(n-3) - 50*a(n-4) - 2*a(n-5) - 48*a(n-6) + 88*a(n-7) -
 *            32*a(n-8) Offset 1: so a(1) = 8, a(2) = 61, …
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: For arbitrary‑precision builds, seq_t must support the usual
 * arithmetic operators (+, -, *, etc.) and be default constructible. */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Initial eight terms (for n = 1 … 8) */
static const seq_t init_vals[8] = {
    8ULL,
    61ULL,
    409ULL,
    2837ULL,
    19776ULL,
    137459ULL,
    955680ULL,
    6645662ULL
};

/* Simple dynamic cache for all previously computed terms */
static seq_t *cache      = NULL;   /* pointer to array of seq_t */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of terms stored (always ≥ cache_cap) */

/* Allocate/initialize cache with the first eight terms */
static void init_cache(void)
{
    if (cache != NULL) return;   /* already initialized */

    cache_cap = 8;
    cache_len = 8;
#ifdef __cplusplus
    cache = new seq_t[cache_cap];
#else
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
#endif
    for (size_t i = 0; i < 8; ++i) {
        cache[i] = init_vals[i];
    }
}

/* Grow the cache to accommodate at least 'needed' entries */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;  /* already large enough */

    size_t new_cap = needed;
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i) {
        new_cache[i] = cache[i];
    }
    delete[] cache;
#else
    new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
#endif
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; we use 0‑based indices in 'cache' */
    init_cache();

    if (n <= 8) {
        return cache[n - 1];
    }

    /* Compute any missing terms up to n */
    if (cache_len < (size_t)n) {
        grow_cache((size_t)n);   /* ensure space */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* recurrence uses previous eight terms */
            seq_t val = cache[i - 2] * 7;          /* 7*a(n-1) */
            val -= cache[i - 3] * 2;              /* -2*a(n-2) */
            val += cache[i - 4] * 19;             /* +19*a(n-3) */
            val -= cache[i - 5] * 50;             /* -50*a(n-4) */
            val -= cache[i - 6] * 2;              /* -2*a(n-5) */
            val -= cache[i - 7] * 48;             /* -48*a(n-6) */
            val += cache[i - 8] * 88;             /* +88*a(n-7) */
            val -= cache[i - 9] * 32;             /* -32*a(n-8) */
            cache[i - 1] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
