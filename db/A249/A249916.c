/*
 * A249916  a(n) = 4*(n - 1) - a(n-3), n >= 3, a(0) = a(1) = 1, a(2) = 5.
 * https://oeis.org/A249916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n - 1) - a(n-3), with a(0)=1, a(1)=1, a(2)=5
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *seq = NULL;
    static idx_t len = 0;   /* number of initialised elements in seq */

    /* Handle the initial three values directly */
    if (n < 3) {
        if (n <= 1) return 1;
        return 5;           /* n == 2 */
    }

    /* If the requested index is not yet computed, grow the cache */
    if (n >= len) {
        /* Determine a new capacity big enough for index 'n' */
        idx_t newCap = (len == 0) ? 8 : len * 2;
        if (newCap < n + 1)
            newCap = n + 1 + ((n + 1) / 10) + 10;

        /* Reallocate the underlying array */
        seq_t *tmp = (seq_t *)realloc(seq, newCap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;

        /* Initialise the first three entries if this is the first allocation */
        if (len == 0) {
            seq[0] = 1;
            seq[1] = 1;
            seq[2] = 5;
            len   = 3;
        }

        /* Compute any missing terms up to the requested index */
        for (idx_t i = len; i <= n; ++i) {
            seq_t val = (seq_t)4 * (seq_t)(i - 1);    /* 4*(i-1) */
            val = val - seq[i - 3];                  /* minus a(i-3) */
            seq[i] = val;
        }
        len = n + 1;
    }

    return seq[n];
}
