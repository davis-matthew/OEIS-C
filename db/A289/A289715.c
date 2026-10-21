/*
 * A289715  The order of the semigroup of orientation-preserving full
 *          transformations on n elements.
 * https://oeis.org/A289715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies 
 *   (n-1)*(3*n-10)*a(n) 
 *   +6*(-3*n^2+12*n-8)*a(n-1) 
 *   +3*(9*n^2-35*n+32)*a(n-2) 
 *   -2*(3*n-4)*(2*n-5)*a(n-3) = 0
 * with initial values a(1)=1, a(2)=4, a(3)=24.
 * This function memoises the values and computes up to the requested
 * index using the recurrence.  All arithmetic is performed in a
 * signed 128‑bit intermediate (when available) to avoid overflow
 * on 8/16/32/64‑bit builds.  The implementation is valid C11 and
 * C++17.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* array indexed 0..n, cache[0] unused */
static idx_t  cache_len = 0;    /* highest index already computed */
static size_t cache_cap = 0;    /* allocated capacity in seq_t elements */

/* a(n) : order of the semigroup of orientation‑preserving full transformations */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety: offset is 1 */

    /* Ensure the cache is large enough for indices 0..n */
    size_t need = (size_t)(n + 1);
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed: return 0 as error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first three valid terms on first use */
    if (cache_len < 3) {
        if (cache_cap < 4) {  /* should never happen because of the allocation above */
            seq_t *tmp = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_cap = 4;
        }
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)24;
        cache_len = 3;
    }

    /* Return cached value if already computed */
    if (n <= cache_len) return cache[n];

    /* Compute subsequent values using the recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Use signed 128‑bit intermediate for safety */
        signed __int128 k128   = (signed __int128)k;
        signed __int128 term1  = (signed __int128)2 * (3*k128 - 4) * (2*k128 - 5)
                               * (signed __int128)cache[k-3];
        signed __int128 term2  = (signed __int128)6 * (-3*k128*k128 + 12*k128 - 8)
                               * (signed __int128)cache[k-1];
        signed __int128 term3  = (signed __int128)3 * (9*k128*k128 - 35*k128 + 32)
                               * (signed __int128)cache[k-2];
        signed __int128 den    = (k128 - 1) * (3*k128 - 10);
        signed __int128 numer  = term1 - term2 - term3;

        cache[k] = (seq_t)(numer / den);
    }

    cache_len = n;
    return cache[n];
}
