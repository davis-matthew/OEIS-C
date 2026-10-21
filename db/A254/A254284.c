/*
 * A254284  Indices of centered triangular numbers (A005448) which are also
 *          hexagonal numbers (A000384).
 * https://oeis.org/A254284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A254284: Indices of centered triangular numbers which are
 * also hexagonal numbers.
 *
 * Recurrence: a(1)=1, a(2)=36, a(3)=133, a(4)=6888, a(5)=25705,
 * a(n)=a(n-1)+194*a(n-2)-194*a(n-3)-a(n-4)+a(n-5)   (n>5) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based indexed cache, cache[0] unused */
    static size_t cache_len = 0;  /* highest index stored so far */

    if (n == 0) return 0;  /* undefined for offset‑1 sequences, return 0 */

    /* initialise cache with the first 5 terms if not already done */
    if (cache_len < 5)
    {
        size_t new_size = 6;                 /* indices 0..5 */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;                       /* allocation failed */
        cache = tmp;
        cache_len = 5;
        cache[0] = 0;                        /* unused but harmless */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)133;
        cache[4] = (seq_t)6888;
        cache[5] = (seq_t)25705;
    }

    /* return cached value if already computed */
    if (n <= cache_len)
        return cache[n];

    /* extend cache up to index n */
    size_t new_size = n + 1;   /* index n will be stored, keep 0 unused */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        return 0;               /* allocation failed */
    cache = tmp;

    /* compute missing values using the recurrence */
    for (size_t idx = cache_len + 1; idx <= n; ++idx)
    {
        seq_t val = cache[idx - 1];                        /* a(n-1) */
        val = val + (seq_t)194 * cache[idx - 2];           /* +194*a(n-2) */
        val = val - (seq_t)194 * cache[idx - 3];           /* -194*a(n-3) */
        val = val - cache[idx - 4];                        /* -a(n-4)   */
        val = val + cache[idx - 5];                        /* +a(n-5)   */
        cache[idx] = val;
    }

    cache_len = n;
    return cache[n];
}
