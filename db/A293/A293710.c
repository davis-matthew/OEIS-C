/*
 * A293710  Expansion of x^2/(1 - 4*x - 4*x^2 - x^3).
 * https://oeis.org/A293710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=0, a(2)=1
   a(n)=4*a(n-1)+4*a(n-2)+a(n-3)  for n ≥ 3
   (Equals the power‑series expansion of  x²/(1‑4·x‑4·x²‑x³) )
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed values */
    static seq_t *cache  = NULL;   /* pointer to array of seq_t */
    static size_t len    = 0;      /* number of already stored entries */

    /* initialise with the first three terms once */
    if (cache == NULL) {
        len    = 3;
        cache  = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);          /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
    }

    /* if already computed, just return */
    if ((size_t)n < len)
        return cache[(size_t)n];

    /* grow array so it can hold indices up to n */
    size_t needed = (size_t)n + 1;
    if (needed > len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;           /* keep the old valid part */
        /* 'len' still contains the size of the computed part */
    }

    /* compute missing terms from len up to n inclusive */
    for (size_t i = len; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)4 * cache[i-1]) +
                   ((seq_t)4 * cache[i-2]) +
                   ((seq_t)1 * cache[i-3]);
    }

    /* now we have indices 0 .. n populated */
    len = needed;
    return cache[(size_t)n];
}
