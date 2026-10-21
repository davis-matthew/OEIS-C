/*
 * A275139  Number of n X 5 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,-2) or (0,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) + a(n-3) for n > 4 Initial values:
 *            a(1)=8, a(2)=144, a(3)=528, a(4)=2320. This recurrence holds for
 *            all n ≥ 5
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* The function defined by the OEIS entry A275139. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* static cache that expands on demand */
    static seq_t *cache   = NULL;   /* array 0 … n (index 0 unused) */
    static idx_t cache_len = 0;     /* memory allocated (size of array) */
    static idx_t computed  = 0;     /* highest index already filled */

    /* Grow the cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 */
            if (cache) free(cache);
            cache = NULL;  cache_len = 0;  computed = 0;
            return 0;
        }
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)8;
        } else if (i == 2) {
            cache[i] = (seq_t)144;
        } else if (i == 3) {
            cache[i] = (seq_t)528;
        } else if (i == 4) {
            cache[i] = (seq_t)2320;
        } else {
            /* recurrence: 3*a(i-1) + 5*a(i-2) + a(i-3) */
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];

            /* compute 3*a1 + 5*a2 + a3 using only additions */
            seq_t r = a1 + a1 + a1;              /* 3*a1 */
            r += a2 + a2 + a2 + a2 + a2;        /* +5*a2 */
            r += a3;                            /* +a3 */
            cache[i] = r;
        }
    }
    computed = n;
    return cache[n];
}
