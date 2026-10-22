/*
 * A302677  Number of nX5 0..1 arrays with every element equal to 0, 1, 4 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + 2*a(n-4) + 2*a(n-5) for n>10 with
 *            initial values a(1)=8, a(2)=7, a(3)=18, a(4)=30, a(5)=63,
 *            a(6)=125, a(7)=257, a(8)=528, a(9)=1085, a(10)=2228
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 60, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache the values computed so far. */
    static seq_t *cache = NULL;      /* cache[i] holds a(i) for i>=1 */
    static size_t cache_len = 0;     /* allocated slots = cache_len (>= n+1) */

    /* Handle small n directly. */
    if (n <= 10)
    {
        static const seq_t init[10] = {8, 7, 18, 30, 63, 125, 257, 528, 1085, 2228};
        return init[(size_t)n - 1];
    }

    /* Grow cache if needed. */
    if (cache_len <= (size_t)n)
    {
        size_t new_len = (size_t)n + 1;          /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();                             /* allocation failure */
        cache = tmp;

        /* Compute missing values, starting at max(cache_len,1). */
        idx_t start = (cache_len > 0) ? cache_len : (idx_t)1;
        for (idx_t i = start; i <= n; ++i)
        {
            if (i <= 10)
            {
                static const seq_t init[10] = {8, 7, 18, 30, 63, 125, 257, 528, 1085, 2228};
                cache[i] = init[(size_t)i - 1];
            }
            else
            {
                cache[i] = cache[i - 1]
                           + ((seq_t)3 * cache[i - 3])
                           + ((seq_t)2 * cache[i - 4])
                           + ((seq_t)2 * cache[i - 5]);
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
