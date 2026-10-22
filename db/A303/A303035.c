/*
 * A303035  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for n > 16 a(n) = 4*a(n-1) - 5*a(n-2) +
 *            16*a(n-3) - 36*a(n-4) + 32*a(n-5) - 63*a(n-6) + 64*a(n-7) -
 *            51*a(n-8) + 4*a(n-9) + 60*a(n-10) + 16*a(n-11) + 112*a(n-12) +
 *            64*a(n-13) Initial values (n = 1 .. 16): 3, 11, 14, 35, 71, 186,
 *            459, 1287, 3490, 9699, 27707, 79358, 229491, 669...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t BASE[16] = {
    3ULL,     11ULL,    14ULL,    35ULL,    71ULL,
    186ULL,   459ULL,   1287ULL,  3490ULL,  9699ULL,
    27707ULL, 79358ULL, 229491ULL,669203ULL,1958414ULL,5753843ULL
};

/* compute sequence A303035 */
seq_t a(idx_t n) {
    /* cache of computed terms, 1-based indexing: cache[0] = a(1), etc. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms stored in cache     */
    /* Ensure cache is initialized with the first 16 terms. */
    if (cache == NULL) {
        /* allocate at least 16 slots – may enlarge later */
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory – caller must handle */
        memcpy(cache, BASE, 16 * sizeof(seq_t));
        cache_len = 16;
    }

    /* grow cache if a larger n is requested */
    if (n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache = tmp;
        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i < n; ++i) {
            /* i is the new index (0‑based), term a(i+1) */
            seq_t v = 0;
            v += 4 * cache[i - 1];               /* a(i)   */
            v -= 5 * cache[i - 2];               /* a(i-1) */
            v += 16 * cache[i - 3];              /* a(i-2) */
            v -= 36 * cache[i - 4];              /* a(i-3) */
            v += 32 * cache[i - 5];              /* a(i-4) */
            v -= 63 * cache[i - 6];              /* a(i-5) */
            v += 64 * cache[i - 7];              /* a(i-6) */
            v -= 51 * cache[i - 8];              /* a(i-7) */
            v += 4 * cache[i - 9];               /* a(i-8) */
            v += 60 * cache[i -10];              /* a(i-9) */
            v += 16 * cache[i -11];              /* a(i-10)*/
            v += 112 * cache[i -12];             /* a(i-11)*/
            v += 64 * cache[i -13];              /* a(i-12)*/
            cache[i] = v;
        }
        cache_len = n;
    }

    /* n is now within the cache limits */
    return cache[n - 1];
}
