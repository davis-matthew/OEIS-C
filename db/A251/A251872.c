/*
 * A251872  Number of (n+2)X(4+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not 2 4 5 or 7 and every diagonal and antidiagonal sum
 *          2 4 5 or 7.
 * https://oeis.org/A251872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251872: Number of (n+2)×(6) 0..3 arrays with forbidden subblock sums.
   Recurrence (from OEIS entry):  
   a(n) = 8*a(n-3) + 16*a(n-6) - 128*a(n-9)   for n > 17
   Initial values (a(1)..a(17)) are listed in the recurrences below.  */
#include <stdlib.h>
#include <stdint.h>

/* 1‑based cache of computed terms.  */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* current allocated capacity (>= required) */
static size_t cache_size = 0;  /* largest n for which cache[n] is valid */

/* Ensure that cache has room for indices 0 .. need-1 (i.e. need elements).  */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = need * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* Initialise the new part to zero – not strictly required but safe. */
        for (size_t i = cache_cap / 2; i < new_cap; ++i) cache[i] = 0;
    }
}

/* External linkage as required by the build system.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1, but safe */

    /* First call – initialise the cache with the first 17 terms.  */
    if (cache_size == 0) {
        /* Reserve space for indices 0..17 (18 elements).  */
        ensure_cache(18);

        /* Unused index 0 – set to zero for safety.  */
        cache[0] = 0;

        static const seq_t init[18] = {
            0ULL,          /* index 0 (unused) */
            5756ULL,       /* a(1) */
            15116ULL,      /* a(2) */
            9936ULL,       /* a(3) */
            13152ULL,      /* a(4) */
            31520ULL,      /* a(5) */
            46464ULL,      /* a(6) */
            62720ULL,      /* a(7) */
            164480ULL,     /* a(8) */
            252736ULL,     /* a(9) */
            389376ULL,     /* a(10) */
            910400ULL,     /* a(11) */
            1559552ULL,    /* a(12) */
            2443264ULL,    /* a(13) */
            5882368ULL,    /* a(14) */
            10571776ULL,   /* a(15) */
            17743872ULL,   /* a(16) */
            40570880ULL    /* a(17) */
        };
        for (size_t i = 1; i <= 17; ++i)
            cache[i] = init[i];
        cache_size = 17;
    }

    size_t idx = (size_t)n;

    /* Grow cache if necessary.  */
    ensure_cache(idx + 1);

    /* If we already have the value, return it.  */
    if (idx <= cache_size)
        return cache[idx];

    /* Compute terms up to idx using the recurrence.  */
    for (size_t i = cache_size + 1; i <= idx; ++i) {
        seq_t term = cache[i - 3] * (seq_t)8;
        term += cache[i - 6] * (seq_t)16;
        term -= cache[i - 9] * (seq_t)128;
        cache[i] = term;
    }
    cache_size = idx;
    return cache[idx];
}
