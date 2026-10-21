/*
 * A282034  Three-set non-British additive number system read by rows.
 * https://oeis.org/A282034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) for n > 16 Initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=50, a(6)=51, a(7)=52,
 *            a(8)=53, a(9)=54, a(10)=5, a(11)=10, a(12)=15, a(13)=20,
 *            a(14)=25, a(15)=100, a(16)=125
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[16] = {
    1, 2, 3, 4,
    50, 51, 52, 53, 54,
    5, 10, 15, 20, 25,
    100, 125
};

static seq_t *cache = NULL;          /* array that holds computed values */
static idx_t cache_alloc = 0;        /* allocated size (in elements)   */
static idx_t cache_len   = 0;        /* number of values stored        */

/* Ensure the cache is initialized with the first 16 sequence elements. */
static void init_cache(void)
{
    cache_alloc = 16;
    cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
    if (!cache) abort();               /* fatal if memory allocation fails */
    for (idx_t i = 0; i < 16; ++i)
        cache[i] = init_vals[i];
    cache_len = 16;
}

/* Grow the cache to hold at least 'need' elements.  Reallocate if necessary. */
static void ensure_cache_size(idx_t need)
{
    if (need <= cache_alloc)
        return;
    /* Double the allocation until we have enough space. */
    idx_t new_alloc = cache_alloc ? cache_alloc : 16;
    while (new_alloc < need)
        new_alloc <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_alloc = new_alloc;
}

/* Extend cached values up to index 'n' (1‑based). */
static void extend_cache(idx_t n)
{
    /* 'n' > cache_len must hold when this is called */
    while (cache_len < n) {
        /* Make sure we have space for the new element. */
        ensure_cache_size(cache_len + 1);

        /* Apply the linear recurrence: a_k = a_{k-1}+a_{k-2}-a_{k-3}. */
        cache[cache_len] =
            cache[cache_len - 1] + cache[cache_len - 2] - cache[cache_len - 3];
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* value for indices below offset is undefined */
    if (!cache) init_cache();           /* initialize on first call */

    if (n > cache_len)
        extend_cache(n);                 /* compute up to requested index */

    return cache[n - 1];                 /* array is 0‑based, offset is 1 */
}
