/*
 * A381513  Expansion of 1/((1-x) * (1-9*x) * (1-25*x) * (1-49*x)).
 * https://oeis.org/A381513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (49^(n+3) - 5*25^(n+3) + 9^(n+4) - 5) / 46080 Recurrence:
 *            a(0)=1, a(1)=84, a(2)=5082, a(3)=273988 a(n) = 84*a(n-1) -
 *            1974*a(n-2) + 12916*a(n-3) - 11025*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

static seq_t *cache      = NULL;   /* array of computed terms       */
static idx_t  cache_len  = 0;      /* number of computed entries    */
static size_t cache_cap  = 0;      /* allocated capacity (>=cache_len) */

/* Return the first four terms explicitly; used for small n. */
static inline seq_t init_term(idx_t n)
{
    switch (n)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)84;
        case 2: return (seq_t)5082;
        case 3: return (seq_t)273988;
        default: return (seq_t)0;
    }
}

/* Ensure cache contains terms 0 … n. Grow it if necessary. */
static void extend_to(idx_t n)
{
    /* Grow the storage if we need more room. */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return;          /* allocation failed – caller will get garbage */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute the initial four terms if they are not yet known. */
    if (cache_len < 4)
    {
        idx_t upto = (n < 3) ? n : 3;
        for (idx_t i = cache_len; i <= upto; ++i)
            cache[i] = init_term(i);
        cache_len = (idx_t)((n < 3) ? n + 1 : 4);
    }

    /* Use the linear recurrence to fill in remaining terms. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        cache[i] =
            (seq_t)84   * cache[i-1] -
            (seq_t)1974 * cache[i-2] +
            (seq_t)12916 * cache[i-3] -
            (seq_t)11025 * cache[i-4];
    }
    cache_len = n + 1;   /* now all terms 0 … n are available */
}

seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];   /* already computed */

    extend_to(n);
    return cache[n];
}
