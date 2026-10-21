/*
 * A274946  Boyd's Pisot-like sequence F(0,5,11).
 * https://oeis.org/A274946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-5)-a(n-6) for n>5; a(0)=0, a(1)=5,
 *            a(2)=11, a(3)=19, a(4)=30, a(5)=46
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 49, u64 n <= 103, u128 n <= 212,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation buffer */
    static size_t cap   = 0;      /* allocated size          */
    static size_t len   = 0;      /* number of terms stored  */

    /* Handle the first six terms directly. */
    if (n < 6)
    {
        switch (n)
        {
            case 0: return (seq_t)0;
            case 1: return (seq_t)5;
            case 2: return (seq_t)11;
            case 3: return (seq_t)19;
            case 4: return (seq_t)30;
            case 5: return (seq_t)46;
        }
    }

    /* Extend the cache if we do not yet have enough space. */
    if ((size_t)n >= cap)
    {
        size_t new_cap = cap ? cap * 2 : 16;
        while ((size_t)n >= new_cap) new_cap <<= 1;

        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure → fallback value */

        if (cache)
        {
            for (size_t i = 0; i < len; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the five base values only once. */
    if (len == 0)
    {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)19;
        cache[4] = (seq_t)30;
        cache[5] = (seq_t)46;
        len = 6;
    }

    /* Compute any missing terms up to the requested index. */
    for (idx_t i = (idx_t)len; i <= n; ++i)
    {
        cache[i] = cache[i-1] + cache[i-3] + cache[i-5] - cache[i-6];
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}
