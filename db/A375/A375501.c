/*
 * A375501  a(n) = Sum_{k=0..n} (-1)^k*A001595(k)^2.
 * https://oeis.org/A375501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 21, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A375501
 *  Recurrence:  a(0..6) = 1,0,9,-16,65,-160,465
 *              a(n)   = -3*a(n-1) + 2*a(n-2) + 8*a(n-3)
 *                       - 2*a(n-4) - 6*a(n-5) + a(n-6) + a(n-7)
 *  for n ≥ 7.
 *  The recurrence follows from the rational generating function
 *  given in the OEIS entry.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoize computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of values stored */

    /* initial values for n = 0 .. 6 */
    static const seq_t init_vals[7] =
        { (seq_t)1, (seq_t)0, (seq_t)9, (seq_t)-16,
          (seq_t)65, (seq_t)-160, (seq_t)465 };

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t cap = 7;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
        cache_size = 7;
    }

    /* expand cache if necessary */
    if ((idx_t)n >= cache_size) {
        idx_t needed = (idx_t)n + 1;
        size_t cap = (size_t)cache_size;

        /* double capacity until it fits needed elements */
        while (cap < needed)
            cap *= 2;

        seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (new_cache == NULL) /* allocation failed; ignore for brevity */
            return init_vals[0];
        cache = new_cache;

        /* compute missing terms up to needed-1 */
        for (idx_t i = cache_size; i < needed; ++i) {
            /* recurrence uses the 7 previous terms; indices safe because i ≥ 7 */
            cache[i] = -3 * cache[i-1] + 2 * cache[i-2] + 8 * cache[i-3]
                       - 2 * cache[i-4] - 6 * cache[i-5]
                       + cache[i-6] + cache[i-7];
        }
        cache_size = needed;
    }

    return cache[n];
}
