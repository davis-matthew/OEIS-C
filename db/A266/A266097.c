/*
 * A266097  Number of nX4 integer arrays with each element equal to the number
 *          of horizontal and antidiagonal neighbors exactly one smaller than
 *          itself.
 * https://oeis.org/A266097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A266097 – Number of n×4 arrays with the required neighbour condition.
 *  Recurrence (holds for n>16):
 *      a(n) =  a(n-1)  + 24·a(n-2) + 17·a(n-3) - 89·a(n-4)
 *           - 40·a(n-5) +146·a(n-6) + 18·a(n-7) - 82·a(n-8)
 *           -  6·a(n-9) -  8·a(n-10)+ 36·a(n-11)- 13·a(n-12)
 *           +  a(n-13)-  2·a(n-14)+ a(n-15)
 *  Initial values (n = 1 … 16):
 *      5, 36, 161, 929, 4893, 26948, 145274, 790986, 4286644,
 *      23281595, 126318252, 685685972, 3721248548,
 *      20197452145, 109618496442, 594950280201
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached sequence values.  Index 0 is unused to keep 1‑based indexing. */
static seq_t *cache     = NULL;   /* pointer to cached terms      */
static size_t cache_cap = 0;      /* allocated number of slots    */
static idx_t  cached_max = 0;     /* highest n computed (>=0)     */

/* Local copy of the first 16 terms.  They all fit in 64 bits, so
 * the cast to seq_t is safe for any build that supports conversion
 * from unsigned 64‑bit integer. */
static const seq_t seedvals[17] = {
    0ULL,                      /* dummy for n = 0 */
    5ULL,                    /* a(1) */
    36ULL,                   /* a(2) */
    161ULL,                  /* a(3) */
    929ULL,                  /* a(4) */
    4893ULL,                 /* a(5) */
    26948ULL,                /* a(6) */
    145274ULL,               /* a(7) */
    790986ULL,               /* a(8) */
    4286644ULL,              /* a(9) */
    23281595ULL,             /* a(10) */
    126318252ULL,            /* a(11) */
    685685972ULL,            /* a(12) */
    3721248548ULL,           /* a(13) */
    20197452145ULL,          /* a(14) */
    109618496442ULL,         /* a(15) */
    594950280201ULL          /* a(16) */
};

/* Ensure the cache has room for indices up to n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, memory is lost – acceptable for this toy
     * implementation; the application would crash for the next call. */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main function.  */
seq_t a(idx_t n)
{
    if (n == 0)            /* out-of-range – define as 0 */
        return (seq_t)0;

    ensure_cache(n);

    /* Already computed? */
    if (cached_max >= n)
        return cache[n];

    /* Seed the first 16 terms if not already done. */
    if (cached_max < 16) {
        idx_t upto = (n < 16) ? n : 16;
        for (idx_t i = cached_max + 1; i <= upto; ++i)
            cache[i] = (seq_t)seedvals[i];
        cached_max = upto;
    }

    /* Compute new terms.  The recurrence requires 15 previous terms,
     * so the first iteration is n = 17. */
    idx_t start = cached_max + 1;
    if (start < 17)
        start = 17;

    for (idx_t i = start; i <= n; ++i) {
        cache[i] =
            cache[i-1] +
            (seq_t)24 * cache[i-2] +
            (seq_t)17 * cache[i-3] -
            (seq_t)89 * cache[i-4] -
            (seq_t)40 * cache[i-5] +
            (seq_t)146 * cache[i-6] +
            (seq_t)18 * cache[i-7] -
            (seq_t)82 * cache[i-8] -
            (seq_t)6 * cache[i-9] -
            (seq_t)8 * cache[i-10] +
            (seq_t)36 * cache[i-11] -
            (seq_t)13 * cache[i-12] +
            cache[i-13] -
            (seq_t)2 * cache[i-14] +
            cache[i-15];
    }

    cached_max = n;
    return cache[n];
}
