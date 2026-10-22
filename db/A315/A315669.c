/*
 * A315669  Coordination sequence Gal.5.113.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 Initial values:
 *            a(0)=1, a(1)=6, a(2)=12, a(3)=17, a(4)=21, a(5)=25, a(6)=29,
 *            a(7)=34, a(8)=40, a(9)=46, a(10)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[11] = {
        1, 6, 12, 17, 21, 25, 29, 34, 40, 46, 52
    };

    /* static state for memoisation */
    static seq_t *cache     = NULL;
    static idx_t  cap       = 0;       /* allocated size (>= 11 when ready) */
    static idx_t  computed  = 10;      /* largest index for which cache contains a value */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 11;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure – unlikely in tests */
        for (idx_t i = 0; i < 11; ++i)
            cache[(size_t)i] = init[i];
    }

    /* quick return for already computed values */
    if (n <= computed)
        return cache[(size_t)n];

    /* grow allocation if needed */
    if (cap <= n) {
        size_t new_cap = (cap == 0) ? 11 : cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[(size_t)i] =
            cache[(size_t)(i - 1)] +
            cache[(size_t)(i - 9)] -
            cache[(size_t)(i - 10)];
    }
    computed = n;

    return cache[(size_t)n];
}
