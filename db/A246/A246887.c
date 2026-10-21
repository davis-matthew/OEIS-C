/*
 * A246887  Number of length n+4 0..3 arrays with some pair in every
 *          consecutive five terms totalling exactly 3.
 * https://oeis.org/A246887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+4*a(n-2)+6*a(n-3)+12*a(n-4)
 *            -4*a(n-5)-6*a(n-6)-2*a(n-8)+a(n-10); Initial values (offset 1):
 *            a(1)=900, a(2)=3364, a(3)=12544, a(4)=46656, a(5)=173056,
 *            a(6)=643204, a(7)=2390116, a(8)=8880400, a(9)=32993536,
 *            a(10)=122589184
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 29, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed values, indices start at 1 */
static idx_t  max_computed = 0;     /* highest n that has been computed */
static size_t cache_size = 0;       /* current allocated size of cache */

/* initial values (index 0 unused) */
static const seq_t base_vals[11] = {
    0ULL,
    900ULL, 3364ULL, 12544ULL, 46656ULL, 173056ULL,
    643204ULL, 2390116ULL, 8880400ULL, 32993536ULL, 122589184ULL
};

/* ensure cache has space for indices 0..needed */
static void ensure_cache_size(idx_t needed)
{
    if (cache_size > (size_t)needed)
        return;
    size_t new_size = cache_size ? cache_size << 1 : 16;
    while (new_size <= (size_t)needed)
        new_size <<= 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* not defined for n < 1, guard anyway */

    /* initialise cache on first call */
    if (cache == NULL)
    {
        ensure_cache_size(16);          /* sufficient for indices up to 10 */
        cache[0] = 0;                   /* unused slot for index 0 */
        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = base_vals[i];
        max_computed = 10;
    }

    if (n <= max_computed)
        return cache[n];

    /* compute values up to n */
    ensure_cache_size(n);
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* recurrence applies for i >= 11; loop starts at 11 because
           max_computed is at least 10 */
        seq_t pos = cache[i-1] * 2
                  + cache[i-2] * 4
                  + cache[i-3] * 6
                  + cache[i-4] * 12;
        seq_t neg = cache[i-5] * 4
                  + cache[i-6] * 6
                  + cache[i-8] * 2;
        cache[i] = pos - neg + cache[i-10];
    }
    max_computed = n;
    return cache[n];
}
