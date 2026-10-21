/*
 * A266027  Number of n X 2 integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors less than itself.
 * https://oeis.org/A266027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2) + 7*a(n-3) – a(n-4) for n>4 with
 *            a(1)=3, a(2)=11, a(3)=55, a(4)=263
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a real program you might handle allocation failure here. */
        exit(1);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* External linkage function as required */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1, but never used */

    size_t sz = (size_t)n;
    grow_cache(sz);

    const seq_t init[4] = { (seq_t)3, (seq_t)11, (seq_t)55, (seq_t)263 };

    for (size_t i = cache_len; i < sz; ++i) {
        seq_t val;
        if (i < 4)
            val = init[i];
        else
            val = 3 * cache[i-1] + 7 * cache[i-2] + 7 * cache[i-3] - cache[i-4];
        cache[i] = val;
    }
    cache_len = sz;

    return cache[n-1];
}
