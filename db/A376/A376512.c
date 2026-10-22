/*
 * A376512  Expansion of e.g.f. exp(x^2 * (1 + x)).
 * https://oeis.org/A376512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(2*a(n-2)+3*(n-2)*a(n-3)), a(0)=1, a(1)=0, a(2)=2,
 *            a(3)=6 (Recurrence from the OEIS entry A376512)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached sequence values */
    static idx_t cache_max = 0;    /* highest index computed so far */

    /* handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    /* compute up to n if needed */
    if (n > cache_max)
    {
        size_t new_size = n + 1;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* initialise base values if not yet present */
        if (cache_max < 3)
        {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)2;
            cache[3] = (seq_t)6;
            cache_max = 3;
        }

        /* extend the cache up to n */
        for (idx_t i = cache_max + 1; i <= n; ++i)
        {
            seq_t part1 = (seq_t)(i - 1);
            seq_t term2 = (seq_t)2 * cache[i - 2];
            seq_t term3 = (seq_t)3 * (seq_t)(i - 2) * cache[i - 3];
            cache[i] = part1 * (term2 + term3);
        }

        cache_max = n;
    }

    return cache[n];
}
