/*
 * A397812  Number of tilings of a 3 X n rectangle, with all four corners
 *          removed, using dominoes and right trominoes.
 * https://oeis.org/A397812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 4*a(n-3) + 11*a(n-4) + 2*a(n-5) for
 *            n >= 7, with initial values a(1)=0, a(2)=1, a(3)=0, a(4)=3,
 *            a(5)=4, a(6)=27
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] unused */
    static size_t cache_size = 0;    /* allocate up to cache_size-1  */
    static idx_t max_computed = 0;   /* largest n already computed */

    if (n < 1)
        return 0;

    /* enlarge cache if necessary */
    if (cache == NULL || (size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;   /* indices 0..n inclusive */
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            return 0;                     /* allocation failed */
        if (cache)
        {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* initial values for n = 1..6 */
    const seq_t init_vals[7] = {0, 0, 1, 0, 3, 4, 27};

    if (max_computed < 6)
    {
        idx_t start = (max_computed < 1) ? 1 : max_computed + 1;
        idx_t end   = (n < 6) ? n : 6;
        for (idx_t i = start; i <= end; ++i)
            cache[i] = init_vals[i];
    }

    /* compute recurrence for n >= 7 */
    if (n >= 7 && max_computed < n)
    {
        idx_t start = (max_computed < 7) ? 7 : max_computed + 1;
        for (idx_t i = start; i <= n; ++i)
        {
            seq_t pos = cache[i-1] * 2
                      + cache[i-2] * 6
                      + cache[i-4] * 11
                      + cache[i-5] * 2;
            pos -= cache[i-3] * 4;
            cache[i] = pos;
        }
    }

    max_computed = max_computed < n ? n : max_computed;

    return cache[n];
}
