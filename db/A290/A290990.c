/*
 * A290990  p-INVERT of the nonnegative integers (A000027), where p(S) = 1 - S
 *          - S^2.
 * https://oeis.org/A290990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) + a(n-4), with a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=5
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values and the highest index computed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_up_to = (idx_t)-1;

    /* Initialise the cache on first use */
    if (cache == NULL) {
        /* At least four elements are needed for the recurrence */
        idx_t init_sz = n + 1;
        if (init_sz < 4) init_sz = 4;
        cache = (seq_t*)malloc((size_t)init_sz * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure – unlikely */

        cache[0] = 0;  /* a(0) */
        cache[1] = 1;  /* a(1) */
        cache[2] = 2;  /* a(2) */
        cache[3] = 5;  /* a(3) */

        cache_size = init_sz;
        computed_up_to = 3;                /* values 0..3 are valid */
    }

    /* Grow the cache if n is beyond the current allocation */
    if (n + 1 > cache_size) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_sz * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure – unlikely */
        cache = tmp;
        cache_size = new_sz;
    }

    /* Compute new values up to n */
    if (n > computed_up_to) {
        idx_t i;
        for (i = computed_up_to + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            cache[i] = 4 * a1 - 5 * a2 + 2 * a3 + a4;
        }
        computed_up_to = n;
    }

    return cache[n];
}
