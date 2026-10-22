/*
 * A300344  Number of n X 2 0..1 arrays with every element equal to 1 or 2
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 2*a(n-4) + a(n-5); *
 *            Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=15, a(5)=37
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] unused, indices 1..n */
static idx_t cache_size = 0;  /* largest index currently stored */

/* Return the n‑th term of A300344 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset guard – behaviour for n<1 is irrelevant */

    if (n > cache_size)
    {
        /* enlarge the cache to hold indices 0..n (0 unused) */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* maintain a placeholder at index 0 */
        if (cache_size < 1) cache[0] = 0;

        /* compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            if (i == 1)          cache[i] = 1;
            else if (i == 2)     cache[i] = 2;
            else if (i == 3)     cache[i] = 6;
            else if (i == 4)     cache[i] = 15;
            else if (i == 5)     cache[i] = 37;
            else
            {
                cache[i] = cache[i-1]
                           + cache[i-2] * (seq_t)2
                           + cache[i-3] * (seq_t)3
                           + cache[i-4] * (seq_t)2
                           + cache[i-5];
            }
        }
        cache_size = n;
    }

    return cache[n];
}
