/*
 * A281236  Solutions y to the negative Pell equation y^2 = 72*x^2 - 332928
 *          with x,y >= 0.
 * https://oeis.org/A281236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n>6; initial values: a(1)=0,
 *            a(2)=288, a(3)=960, a(4)=1632, a(5)=2688, a(6)=6048
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 29, u64 n <= 66, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms (1‑based indexing: cache[0] == a(1)) */
    static seq_t *cache = NULL;
    static size_t size  = 0;

    /* Initial values for the first six terms. */
    static const seq_t init[6] = {0, 288, 960, 1632, 2688, 6048};

    /* n is expected to be >= 1; 0 is out of range but handled gracefully. */
    if (n == 0) return 0;

    /* Expand cache if necessary. */
    if ((size_t)n > size) {
        /* Double size or grow to n, whichever is larger. */
        size_t newsize = size ? size * 2 : 8;
        if (newsize < (size_t)n) newsize = n;

        /* realloc may return NULL only if newsize is 0; otherwise failure is fatal. */
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp && newsize) return 0;  /* allocation failure */

        cache = tmp;

        /* Initialize new entries. */
        for (size_t i = size; i < newsize; ++i) {
            if (i < 6)
                cache[i] = init[i];
            else
                cache[i] = 6 * cache[i - 3] - cache[i - 6];
        }

        size = newsize;
    }

    /* Return the requested value (1‑based index). */
    return cache[n - 1];
}
