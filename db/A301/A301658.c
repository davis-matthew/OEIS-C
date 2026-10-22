/*
 * A301658  Number of nX4 0..1 arrays with every element equal to 0, 1 or 4
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-a(n-3)-2*a(n-4)+a(n-5) (n>5) Initial
 *            values: a(1)=5, a(2)=6, a(3)=9, a(4)=13, a(5)=24
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static seq_t *cache          = NULL;  /* array of computed terms, cache[0] unused */
static idx_t  cache_capacity = 0;      /* allocated size (including index 0) */
static idx_t  max_computed   = 0;      /* largest n for which a(n) is in cache */

/* ensure that cache has room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < cache_capacity)
        return;                          /* already enough storage */

    size_t new_cap = (size_t)(n + 1);    /* +1 to keep index 0 usable */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache,
               (size_t)cache_capacity * sizeof(seq_t));
        free(cache);
    }
    /* zero any new elements so that subtraction on uninitialized data
       would be safe – they will be overwritten before use. */
    if (cache_capacity < new_cap) {
        size_t unused = new_cap - cache_capacity;
        memset(new_cache + cache_capacity, 0,
               unused * sizeof(seq_t));
    }
    if (cache_capacity == 0)            /* ensure cache[0] == 0 */
        new_cache[0] = (seq_t)0;
    cache          = new_cache;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)                        /* behavior for n<1 is irrelevant */
        return (seq_t)0;

    ensure_capacity(n);

    /* compute all missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)5;
        else if (i == 2)
            cache[2] = (seq_t)6;
        else if (i == 3)
            cache[3] = (seq_t)9;
        else if (i == 4)
            cache[4] = (seq_t)13;
        else if (i == 5)
            cache[5] = (seq_t)24;
        else {
            /* compute 2*a(i-1)+a(i-2)-a(i-3)-2*a(i-4)+a(i-5) */
            seq_t pos = cache[i - 1];              /* a(i-1) */
            pos += cache[i - 1];                  /* 2*a(i-1) */
            pos += cache[i - 2];                  /* + a(i-2) */
            pos += cache[i - 5];                  /* + a(i-5) */

            seq_t neg = cache[i - 3];              /* a(i-3) */
            neg += cache[i - 4];                  /* + a(i-4) */
            neg += cache[i - 4];                  /* + a(i-4) again */

            cache[i] = pos - neg;                 /* final result */
        }
    }
    max_computed = n;
    return cache[n];
}
