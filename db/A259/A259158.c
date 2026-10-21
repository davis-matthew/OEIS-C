/*
 * A259158  Positive triangular numbers (A000217) that are heptagonal numbers
 *          (A000566) divided by 2.
 * https://oeis.org/A259158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T_m where T_m = m(m+1)/2 is triangular and 2*T_m is
 *            heptagonal. Generation uses the Pell equation x^2 - 10 y^2 = -1.
 *            Valid solutions satisfy x ≡ 7 (mod 10). For each such solution,
 *            y = 2m+1 → m = (y-1)/2 → a(n) = m(m+1)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t curX = 0;          /* current x of Pell equation (x^2 - 10 y^2 = -1) */
    static seq_t curY = 0;          /* current y = 2m+1 */
    static seq_t *cache = NULL;     /* cached sequence values */
    static size_t cache_cap = 0;    /* allocated capacity of cache */
    static size_t cache_len = 0;    /* number of values stored in cache */
    static int initialized = 0;     /* flag to perform one–time initialization */

    if (!initialized) {
        curX = (seq_t)3;            /* first solution of x^2 - 10 y^2 = -1 */
        curY = (seq_t)1;
        cache_cap = 4;
        cache_len = 0;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        initialized = 1;
    }

    /* Generate terms until we have at least n terms available */
    while (cache_len < n) {
        /* Next solution of the -1 Pell equation via multiplication by
           the fundamental solution of x^2 - 10 y^2 = 1 (19 + 6√10). */
        seq_t nextX = (seq_t)19 * curX + (seq_t)60 * curY; /* 19x + 60y */
        seq_t nextY = (seq_t)6  * curX + (seq_t)19 * curY; /* 6x  + 19y */
        curX = nextX;
        curY = nextY;

        /* Keep only solutions where x ≡ 7 (mod 10).  Those give integer k. */
        if ((curX % (seq_t)10) == (seq_t)7) {
            seq_t m = (curY - (seq_t)1) / (seq_t)2;      /* m = (y-1)/2 */
            seq_t val = m * (m + (seq_t)1) / (seq_t)2;   /* triangular number */

            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    exit(1);          /* memory allocation failed */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];
}
