/*
 * A268882  Number of n X 4 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 88*a(n-2) + 200*a(n-3) - 208*a(n-4) +
 *            96*a(n-5) - 16*a(n-6) (for n > 7) Initial values: a(1)=5,
 *            a(2)=54, a(3)=501, a(4)=4133, a(5)=31956, a(6)=236960,
 *            a(7)=1706732
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[7] = {5ULL, 54ULL, 501ULL, 4133ULL,
                                   31956ULL, 236960ULL, 1706732ULL};

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cached_max = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Ensure the cache holds the first 7 values */
    if (cache == NULL)
    {
        cache_size = (idx_t)8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL)
            return 0;            /* out of memory */
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
        cached_max = 7;
    }

    if (n <= cached_max)
        return cache[n - 1];

    /* Resize cache if needed */
    if (n > cache_size)
    {
        idx_t new_size = cache_size;
        while (new_size < n)
            new_size <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL)
            return 0;        /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i)
    {
        seq_t a1 = cache[i - 2];  /* a(i-1) */
        seq_t a2 = cache[i - 3];  /* a(i-2) */
        seq_t a3 = cache[i - 4];  /* a(i-3) */
        seq_t a4 = cache[i - 5];  /* a(i-4) */
        seq_t a5 = cache[i - 6];  /* a(i-5) */
        seq_t a6 = cache[i - 7];  /* a(i-6) */

        cache[i - 1] =
            (seq_t)16 * a1
            - (seq_t)88 * a2
            + (seq_t)200 * a3
            - (seq_t)208 * a4
            + (seq_t)96 * a5
            - (seq_t)16 * a6;
    }

    cached_max = n;
    return cache[n - 1];
}
