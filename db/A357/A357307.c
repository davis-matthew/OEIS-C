/*
 * A357307  a(0) = 1, a(1) = 0, a(2) = 1; a(n) = a(n-1) + Sum_{k=0..n-3} a(k)
 *          * a(n-k-3).
 * https://oeis.org/A357307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=1; a(n) = a(n-1) + Sum_{k=0}^{n-3} a(k) *
 *            a(n-k-3) (n ≥ 3)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;      /* memoisation array */
static size_t cache_size = 0;    /* current allocated size (number of elements) */
static idx_t last_computed = 0;  /* highest index that has been computed */

/* Ensure the cache can hold at least (n+1) entries.
   Grow by doubling until sufficient. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_size) return;
    size_t new_size = cache_size ? cache_size * 2 : 8;
    while (new_size <= (size_t)n) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_size = new_size;
}

/* Initialise base values a(0), a(1), a(2). */
static void init_base(void)
{
    if (!cache || cache_size < 3)
    {
        size_t new_size = 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    last_computed = 2;
}

/* Return a(n) for any n ≥ 0.  The function is thread-unsafe but
   satisfies all requested properties. */
seq_t a(idx_t n)
{
    if (cache == NULL || last_computed < 2)
        init_base();

    if (n <= last_computed)
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        seq_t sum = cache[i - 1];          /* a(i-1) */
        for (idx_t k = 0; k <= i - 3; ++k)
            sum += cache[k] * cache[i - k - 3];
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
