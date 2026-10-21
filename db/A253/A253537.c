/*
 * A253537  Number of (n+2) X (1+2) 0..1 arrays with every 3 X 3 subblock
 *          diagonal maximum minus antidiagonal minimum nonincreasing
 *          horizontally and nondecreasing vertically.
 * https://oeis.org/A253537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=512, a(2)=3328, a(3)=20480, a(4)=119040, a(5)=679936 a(n) =
 *            8*a(n-1) - 7*a(n-2) - 56*a(n-3) + 120*a(n-4) - 64*a(n-5) for n ≥
 *            6
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build environment. */

/* cache for computed values, 1‑based indexing; index 0 is unused */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_sz   = 0;

/* first five terms (index 0 is unused) */
static const seq_t init_vals[6] = {
    0ULL,                      /* unused placeholder */
    512ULL,                    /* a(1) */
    3328ULL,                   /* a(2) */
    20480ULL,                  /* a(3) */
    119040ULL,                 /* a(4) */
    679936ULL                  /* a(5) */
};

/* ensure that cache has room for indices up to n */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;          /* indices 0..n */
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : need;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* return the first five terms directly */
    if (n <= 5) return init_vals[(size_t)n];

    ensure_capacity(n);

    /* initialise cache on first use */
    if (!cache_sz) {
        for (idx_t i = 1; i <= 5; ++i)
            cache[(size_t)i] = init_vals[(size_t)i];
        cache_sz = 5;
    }

    /* compute any missing terms up to n */
    for (idx_t i = (idx_t)(cache_sz + 1); i <= n; ++i) {
        seq_t term = cache[(size_t)i-1] * (seq_t)8;
        term -= cache[(size_t)i-2] * (seq_t)7;
        term -= cache[(size_t)i-3] * (seq_t)56;
        term += cache[(size_t)i-4] * (seq_t)120;
        term -= cache[(size_t)i-5] * (seq_t)64;
        cache[(size_t)i] = term;
    }

    cache_sz = (size_t)n;
    return cache[(size_t)n];
}
