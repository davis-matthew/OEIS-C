/*
 * A369809  Expansion of 1/(1 - x^6/(1-x)^7).
 * https://oeis.org/A369809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 6*a(n-6) + a(n-7) Initial values: a(0)=1, a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=0, a(5)=0, a(6)=1, a(7)=7
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed values */
static idx_t  cached_cnt = 0;      /* next index to compute (size of valid data) */
static size_t cached_cap = 0;      /* allocated capacity */

/* Grow cache to at least size n+1 */
static void grow_cache(idx_t n)
{
    if (n < cached_cap)
        return;

    size_t new_cap = (cached_cap == 0 ? 16 : cached_cap);
    while (new_cap <= (size_t)n)
        new_cap <<= 1;   /* multiply by 2 */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) /* allocation failed */ abort();
    cache = new_cache;
    cached_cap = new_cap;
}

/* Initialise the cache with known values up to a(7) */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cached_cap = 16;
    cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
    if (!cache) abort();

    cache[0] = (seq_t)1;
    cache[1] = cache[2] = cache[3] = cache[4] = cache[5] = (seq_t)0;
    cache[6] = (seq_t)1;
    cache[7] = (seq_t)7;

    cached_cnt = 8; /* next index to compute */
}

/* Compute a(n) with memoized recurrence */
seq_t a(idx_t n)
{
    init_cache();

    if (n < cached_cnt)
        return cache[n];

    grow_cache(n);
    for (idx_t i = cached_cnt; i <= n; ++i)
    {
        seq_t t = (seq_t)7  * cache[i - 1];
        t -= (seq_t)21 * cache[i - 2];
        t += (seq_t)35 * cache[i - 3];
        t -= (seq_t)35 * cache[i - 4];
        t += (seq_t)21 * cache[i - 5];
        t -= (seq_t)6  * cache[i - 6];
        t += cache[i - 7];
        cache[i] = t;
    }

    cached_cnt = n + 1;
    return cache[n];
}
