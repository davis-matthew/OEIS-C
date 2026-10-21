/*
 * A251429  Number of length 2+2 0..n arrays with the sum of the maximum minus
 *          twice the median plus the minimum of adjacent triples multiplied
 *          by some arrangement of +-1 equal to zero.
 * https://oeis.org/A251429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - a(n-4) - a(n-5) - a(n-6) - a(n-7) * +
 *            2*a(n-8) + a(n-10) - a(n-11) * Initial values: a(1)=12, a(2)=41,
 *            a(3)=116, a(4)=237, a(5)=432, * a(6)=725, a(7)=1128, a(8)=1641,
 *            a(9)=2316, a(10)=3145, a(11)=4148
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored in cache (>=0) */
static size_t cache_cap = 0;   /* capacity of cache array */

/* Ensure cache is large enough to hold terms up to index n-1 (zero‑based). */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) /* first time: allocate space for initial 11 terms */
    {
        cache_cap = 11 > n ? 11 : n + 4;            /* safety margin */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();                /* memory allocation failure */

        static const seq_t init[11] = {
            12, 41, 116, 237, 432, 725, 1128,
            1641, 2316, 3145, 4148
        };
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cache_len = 11;
    }

    if ((idx_t)cache_len >= n)
        return;

    /* grow cache until it can hold index n-1 */
    if ((size_t)n > cache_cap)
    {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = n + 4;                /* safety margin */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i)
    {
        /* Indices in cache: a(k) is stored at cache[k-1]  (k >= 1) */
        long long t = 0;
        t += (long long)cache[i-1];                 /* a(n-1)  */
        t += 2LL * (long long)cache[i-3];           /* 2*a(n-3)  */
        t -= (long long)cache[i-4];                 /* -a(n-4)   */
        t -= (long long)cache[i-5];                 /* -a(n-5)   */
        t -= (long long)cache[i-6];                 /* -a(n-6)   */
        t -= (long long)cache[i-7];                 /* -a(n-7)   */
        t += 2LL * (long long)cache[i-8];           /* +2*a(n-8) */
        t += (long long)cache[i-10];                /* +a(n-10)  */
        t -= (long long)cache[i-11];                /* -a(n-11)  */
        cache[i] = (seq_t)t;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* offset is 1; for safety */
    ensure_capacity(n);
    return cache[n-1];
}
