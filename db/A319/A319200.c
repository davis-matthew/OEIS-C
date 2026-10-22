/*
 * A319200  a(n) = -(A(n) - A(n-1)) where A(n) = A057597(n+1), for n >= 0.
 * https://oeis.org/A319200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - a(n-2) + a(n-3), for n >= 3 Initial values:
 *            a(0)=0, a(1)=-1, a(2)=2
 * Range:     i8 n <= 17, i16 n <= 34, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed a(i) values */
static idx_t cache_len = 0;      /* current length of cache (meaningful indices 0 .. cache_len-1) */

/* Ensure the cache holds values up to index n (inclusive). */
static void grow_to(idx_t n)
{
    if (n < cache_len)
        return;

    idx_t old_len = cache_len;          /* length before growth */
    idx_t new_len = n + 1;

    seq_t *tmp;
#ifdef __cplusplus
    /* In C++ create a new array and copy the old values. */
    tmp = new seq_t[new_len];
    if (cache) {
        for (idx_t i = 0; i < old_len; ++i)
            tmp[i] = cache[i];
        delete[] cache;
    }
#else
    /* In C use realloc to extend the array. */
    tmp = realloc(cache, new_len * sizeof(seq_t));
#endif
    if (!tmp)
        abort();                       /* out of memory */

    cache = tmp;
    cache_len = new_len;

    /* Initialise any new entries. */
    for (idx_t i = old_len; i < cache_len; ++i) {
        if (i == 0)
            cache[i] = 0;              /* a(0) */
        else if (i == 1)
            cache[i] = -1;             /* a(1) */
        else if (i == 2)
            cache[i] = 2;              /* a(2) */
        else
            cache[i] = -cache[i - 1] - cache[i - 2] + cache[i - 3];
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    grow_to(n);
    return cache[n];
}
