/*
 * A279897  Number of n X 3 0..2 arrays with no element unequal to a strict
 *          majority of its king-move neighbors, with the exception of exactly
 *          one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A279897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=6, a(3)=9, a(4)=20, a(5)=43, a(6)=94, * a(7)=213, *
 *            a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) - 4*a(n-4) (n > 7)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <cstring>
#endif

seq_t a(idx_t n)
{
    /* Cached values: cache[i] holds a(i) for i ≥ 1, cache[0] unused.   */
    static seq_t *cache = NULL;
    static idx_t last = 0;   /* highest index currently cached */

    if (n < 1) return (seq_t)0;        /* undefined for n<1 */

    /* Ensure capacity for index n. */
    if (n > last)
    {
        /* Determine new capacity: at least n+1, grow a bit more for future calls. */
        idx_t new_cap = n + 1;
        if (new_cap < 8) new_cap = 8;        /* enough for the first 7 bases */

        if (cache == NULL)
        {
#ifdef __cplusplus
            cache = new seq_t[new_cap];
#else
            cache = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
#endif
            if (cache == NULL) return (seq_t)0;   /* allocation failure */
        }
        else if (new_cap > last + 1)
        {
#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_cap];
            for (idx_t i = 1; i <= last; ++i) tmp[i] = cache[i];
            delete[] cache;
            cache = tmp;
#else
            seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
#endif
        }

        /* Initialize the first seven terms if they are not yet set. */
        if (last < 1)   cache[1] = (seq_t)2;
        if (last < 2)   cache[2] = (seq_t)6;
        if (last < 3)   cache[3] = (seq_t)9;
        if (last < 4)   cache[4] = (seq_t)20;
        if (last < 5)   cache[5] = (seq_t)43;
        if (last < 6)   cache[6] = (seq_t)94;
        if (last < 7)   cache[7] = (seq_t)213;

        /* Compute missing values using the recurrence. */
        for (idx_t i = last + 1; i <= n; ++i)
        {
            if (i > 7)
            {
                seq_t term =
                    cache[i-1] * (seq_t)2            +
                    cache[i-2] * (seq_t)3            -
                    (cache[i-3] * (seq_t)4 + cache[i-4] * (seq_t)4);
                cache[i] = term;
            }
            /* terms 1..7 are already initialised */
        }

        last = n;
    }

    return cache[n];
}
