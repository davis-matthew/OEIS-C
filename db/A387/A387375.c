/*
 * A387375  Number of cycles in the n-Cameron graph.
 * https://oeis.org/A387375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)-21*a(n-2)+16*a(n-3)-4*a(n-4), a(0) = 3, a(1) =
 *            47, a(2) = 397, a(3) = 3031
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
extern seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t last_computed = 0;  /* highest index already computed */

    /* initialise cache with first four values */
    if (cache == NULL) {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) abort();  /* allocation failed */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)47;
        cache[2] = (seq_t)397;
        cache[3] = (seq_t)3031;
        last_computed = 3;
    }

    /* Expand cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while ((size_t)n + 1 > new_cap)
            new_cap = new_cap ? new_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* Return already computed value */
    if (n <= last_computed)
        return cache[n];

    /* Compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] =
            ((seq_t)10) * cache[i - 1]
            - ((seq_t)21) * cache[i - 2]
            + ((seq_t)16) * cache[i - 3]
            - ((seq_t)4)  * cache[i - 4];
    }
    last_computed = n;
    return cache[n];
}
