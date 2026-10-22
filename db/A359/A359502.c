/*
 * A359502  a(n) = a(n-2)^2 + a(n-1) + 1 for n >= 2 with a(0) = 0 and a(1) =
 *          1.
 * https://oeis.org/A359502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)^2 + a(n-1) + 1, a(0)=0, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache over all calls */
    static seq_t *cache = NULL;
    static size_t cap = 0;         /* allocated size of cache   */
    static size_t computed = 0;    /* number of terms filled   */

    /* First allocation with room for the two base cases */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out‑of‑memory – return 0 */
        cache[0] = 0;
        cache[1] = 1;
        computed = 2;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;           /* double until we can hold n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* out‑of‑memory – return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Fill missing terms up to n */
    while (computed <= (size_t)n) {
        seq_t prev1 = cache[computed - 1];
        seq_t prev2 = cache[computed - 2];
        cache[computed] = (prev2 * prev2) + prev1 + 1;
        ++computed;
    }

    return cache[n];
}
