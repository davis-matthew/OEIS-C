/*
 * A302946  Number of minimal (and minimum) total dominating sets in the
 *          2n-crossed prism graph.
 * https://oeis.org/A302946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-2) - a(n-3) (a(1)=4, a(2)=36, a(3)=196)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* assume n >= 1 */
    static seq_t *cache = NULL;   /* indices 0..n, index 0 unused */
    static idx_t cap = 0;         /* allocated size in terms of n+1 (>=4) */
    static idx_t len = 0;         /* highest term that has been computed (>=3 after init) */

    /* initialise cache on the first call */
    if (cache == NULL) {
        cap = 4;  /* indices 0..3 */
        cache = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)196;
        len = 3;
    }

    /* enlarge storage if the requested index is beyond current capacity */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (idx_t i = cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* compute missing terms up to the requested n */
    while (len < n) {
        idx_t i = len + 1;
        cache[i] = (seq_t)5 * cache[i-1] + (seq_t)5 * cache[i-2] - cache[i-3];
        ++len;
    }

    return cache[n];
}
