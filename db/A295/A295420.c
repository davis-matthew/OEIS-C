/*
 * A295420  Number of total dominating sets in the n-Moebius ladder.
 * https://oeis.org/A295420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-3) - 2*a(n-4) + 10*a(n-5) + 4*a(n-6) -
 *            a(n-8) - a(n-9) (n > 9) Initial values (offset 1): a(1)=1,
 *            a(2)=11, a(3)=49, a(4)=131, a(5)=441, a(6)=1499, a(7)=5041,
 *            a(8)=17155, a(9)=58081
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; value for n=0 unused */

    /* Static cache for memoised values */
    static seq_t *cache = NULL;      /* array indices 0 .. cap-1, ignore index 0 */
    static size_t cap   = 0;         /* allocated size */
    static size_t len   = 0;         /* last index with a value computed */

    /* Initial values a(1)..a(9) */
    static const seq_t init[10] = {
        0ULL,          /* index 0 unused */
        1ULL,          /* a(1) */
        11ULL,         /* a(2) */
        49ULL,         /* a(3) */
        131ULL,        /* a(4) */
        441ULL,        /* a(5) */
        1499ULL,       /* a(6) */
        5041ULL,       /* a(7) */
        17155ULL,      /* a(8) */
        58081ULL       /* a(9) */
    };

    /* Return a pre‑computed term directly for n <= 9 */
    if (n <= 9) return init[(size_t)n];

    /* Ensure the cache is large enough to store up to index n */
    if (cache == NULL) {
        /* Allocate space for indices 0 .. n (size n+1) or at least 10 */
        cap = (size_t)n + 1;
        if (cap < 10) cap = 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, unlikely */
        /* Copy the known initial values */
        for (size_t i = 1; i <= 9; ++i) cache[i] = init[i];
        len = 9;
    } else if ((size_t)n >= cap) {
        /* Grow the cache to accommodate index n */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[0]; /* allocation failure, unlikely */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n (only needed for n>9) */
    for (size_t k = len + 1; k <= (size_t)n; ++k) {
        seq_t val = (seq_t)3 * cache[k - 1]
                  + (seq_t)4 * cache[k - 3]
                  - (seq_t)2 * cache[k - 4]
                  + (seq_t)10 * cache[k - 5]
                  + (seq_t)4 * cache[k - 6]
                  - cache[k - 8]
                  - cache[k - 9];
        cache[k] = val;
    }
    len = (size_t)n;
    return cache[(size_t)n];
}
