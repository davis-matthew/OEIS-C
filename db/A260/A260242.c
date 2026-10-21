/*
 * A260242  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000101 or 00001001.
 * https://oeis.org/A260242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) +4*a(n-3) +9*a(n-4) -13*a(n-5) +14*a(n-6)
 *            +6*a(n-7) -35*a(n-8) +11*a(n-9) +11*a(n-10) -4*a(n-11)
 *            -14*a(n-12) +10*a(n-13) -4*a(n-14) for n>15 Initial values:
 *            a(1)=99, a(2)=213, a(3)=551, a(4)=1235, a(5)=3026, a(6)=7822,
 *            a(7)=18837, a(8)=45114, a(9)=113177, a(10)=278981, a(11...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 20, u64 n <= 45, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;           /* array of computed values */
static idx_t  cache_cap = 0;          /* allocated capacity (elements)   */
static idx_t  cache_size = 0;         /* largest index with computed value */

static void
ensure_capacity(idx_t needed)
{
    if (needed < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= needed)
        new_cap *= 2;

    seq_t *new_cache;
#if defined(__cplusplus)
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
#endif
    if (!new_cache)
        abort();   /* allocation failure */

    if (cache) {
        /* copy old data */
        for (idx_t i = 0; i <= cache_size; ++i)
            new_cache[i] = cache[i];
#if defined(__cplusplus)
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, but harmless */

    /* initial load of the first 15 terms */
    if (!cache) {
        ensure_capacity(15);
        /* zero the array for safety */
        memset((void *)cache, 0, (size_t)cache_cap * sizeof(seq_t));

        static const seq_t init[16] = {
            (seq_t)0,         /* dummy for 0‑index */
            (seq_t)99,
            (seq_t)213,
            (seq_t)551,
            (seq_t)1235,
            (seq_t)3026,
            (seq_t)7822,
            (seq_t)18837,
            (seq_t)45114,
            (seq_t)113177,
            (seq_t)278981,
            (seq_t)673881,
            (seq_t)1658848,
            (seq_t)4103482,
            (seq_t)10010581,
            (seq_t)24491986
        };
        for (idx_t i = 1; i <= 15; ++i)
            cache[i] = init[i];
        cache_size = 15;
    }

    if (n <= cache_size)       /* already known */
        return cache[n];

    /* grow cache to hold up to n */
    ensure_capacity(n);

    /* compute new terms from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = cache[i-1]
                  + cache[i-2]
                  + (seq_t)4 * cache[i-3]
                  + (seq_t)9 * cache[i-4]
                  + (seq_t)14 * cache[i-6]
                  + (seq_t)6  * cache[i-7]
                  + (seq_t)11 * cache[i-9]
                  + (seq_t)11 * cache[i-10]
                  + (seq_t)10 * cache[i-13];

        seq_t neg = (seq_t)13 * cache[i-5]
                  + (seq_t)35 * cache[i-8]
                  + (seq_t)4  * cache[i-11]
                  + (seq_t)14 * cache[i-12]
                  + (seq_t)4  * cache[i-14];

        cache[i] = pos - neg;   /* guaranteed non‑negative with the given recurrence */
    }

    cache_size = n;
    return cache[n];
}
