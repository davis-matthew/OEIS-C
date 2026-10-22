/*
 * A301403  Number of nX4 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 5 horizontally, vertically or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A301403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) +40*a(n-2) +9*a(n-3) -29*a(n-4) +4*a(n-5), with
 *            a(1)=8, a(2)=128, a(3)=1988, a(4)=31012, a(5)=483600
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* memoization array, 1‑based */
static size_t cache_size = 0;    /* allocated size */

/* Ensure the cache can hold index n (1‑based).  New slots are zero‑initialized. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;
    size_t new_size = (cache_size == 0) ? 8 : cache_size * 2;
    if (new_size < (size_t)n + 1)
        new_size = (size_t)n + 1;
    seq_t *tmp = (cache_size == 0) ? (seq_t *)malloc(new_size * sizeof(seq_t))
                                   : (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) /* allocation failed – let the caller crash naturally */
        return;
    /* Zero‑initialize the newly allocated part */
    if (new_size > cache_size)
        memset(tmp + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1; safe return */

    ensure_cache(n);
    if (cache[n] != 0)
        return cache[n];   /* already computed */

    /* Compute all needed terms up to n in ascending order */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != 0)
            continue;       /* already computed earlier in the loop */

        if (i == 1)
            cache[1] = 8;
        else if (i == 2)
            cache[2] = 128;
        else if (i == 3)
            cache[3] = 1988;
        else if (i == 4)
            cache[4] = 31012;
        else if (i == 5)
            cache[5] = 483600;
        else
        {
            seq_t t = 13 * cache[i - 1]
                    + 40 * cache[i - 2]
                    + 9  * cache[i - 3];
            t -= 29 * cache[i - 4];
            t += 4  * cache[i - 5];
            cache[i] = t;
        }
    }
    return cache[n];
}
