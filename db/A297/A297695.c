/*
 * A297695  Number of 2Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A297695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) – 6*a(n-3), with a(1)=4, a(2)=8,
 *            a(3)=26
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cache for previously computed values.  cache[i-1] contains a(i). */
static seq_t *cache = NULL;    /* cache array  */
static idx_t cached = 0;       /* highest n already computed  */
static size_t cap = 0;         /* allocated size of cache      */

seq_t a(idx_t n) {
    if (n < 1) {
        return (seq_t)0; /* undefined for n < 1, but return 0 safely */
    }

    /* Ensure capacity for indices up to n. */
    if ((size_t)n > cap) {
        size_t new_cap = (cap) ? cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialize base cases only once. */
    if (cached == 0) {
        /* guarantee space for the first three terms */
        if (cap < 3) {
            size_t need = 3;
            seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cap = need;
        }
        cache[0] = 4;  /* a(1) */
        cache[1] = 8;  /* a(2) */
        cache[2] = 26; /* a(3) */
        cached = 3;
    }

    /* Compute missing terms up to n. */
    if (n > cached) {
        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t a1 = cache[(size_t)(i - 2)]; /* a(i-1) */
            seq_t a2 = cache[(size_t)(i - 3)]; /* a(i-2) */
            seq_t a3 = cache[(size_t)(i - 4)]; /* a(i-3) */
            cache[(size_t)(i - 1)] = 2 * a1 + 4 * a2 - 6 * a3;
        }
        cached = n;
    }

    return cache[(size_t)(n - 1)];
}
