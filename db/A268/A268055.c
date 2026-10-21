/*
 * A268055  Number of nX7 0..2 arrays with new values introduced in each row
 *          and column in sequential order starting with zero.
 * https://oeis.org/A268055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 549*a(n-1) - 75693*a(n-2) + 3237331*a(n-3) -
 *            45379926*a(n-4) + 206208420*a(n-5) - 255602200*a(n-6) Initial
 *            values: a(1)=1, a(2)=64, a(3)=10438, a(4)=2796826,
 *            a(5)=915745002, a(6)=322124540754
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises the sequence in a static cache so that
   repeated calls with any order of n are fast. */

/* Standard headers for dynamic allocation and size_t */
#include <stdlib.h>
#include <stddef.h>

/* Helper: ensure the cache can hold indices up to n (1‑based) */
static void
ensure_cache(idx_t n, seq_t **cache, size_t *cap)
{
    if (*cache == NULL) {
        size_t init = 64;                     /* initial capacity */
        if (n + 1 > init) init = n + 1;       /* at least n+1 */
        *cache = (seq_t*)malloc(init * sizeof(seq_t));
        *cap   = init;
    }
    if ((size_t)(n + 1) > *cap) {
        size_t newcap = *cap;
        while ((size_t)(n + 1) > newcap) newcap <<= 1;
        seq_t *newp = (seq_t*)malloc(newcap * sizeof(seq_t));
        for (size_t i = 0; i < *cap; ++i) newp[i] = (*cache)[i];
        free(*cache);
        *cache = newp;
        *cap   = newcap;
    }
}

/* The sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cap  = 0;      /* capacity of cache array */

    /* Ensure we have storage for at least n */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first six terms if they are not yet set */
    if (cap < 7) ensure_cache(6, &cache, &cap);   /* guarantees space */

    if (cache[1] == 0) {               /* zero indicates uninitialised */
        cache[1] = (seq_t)1ULL;
        cache[2] = (seq_t)64ULL;
        cache[3] = (seq_t)10438ULL;
        cache[4] = (seq_t)2796826ULL;
        cache[5] = (seq_t)915745002ULL;
        cache[6] = (seq_t)322124540754ULL;
    }

    /* Compute any missing terms up to n */
    for (size_t i = 7; i <= (size_t)n; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        seq_t v4 = cache[i - 4];
        seq_t v5 = cache[i - 5];
        seq_t v6 = cache[i - 6];

        seq_t res = (seq_t)549 * v1;
        res = res - (seq_t)75693 * v2;
        res = res + (seq_t)3237331 * v3;
        res = res - (seq_t)45379926 * v4;
        res = res + (seq_t)206208420 * v5;
        res = res - (seq_t)255602200 * v6;

        cache[i] = res;
    }

    return cache[(size_t)n];
}
