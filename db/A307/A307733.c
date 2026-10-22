/*
 * A307733  a(0) = a(1) = 1; a(n) = a(n-1) + a(n-2) + Sum_{k=0..n-1} a(k) *
 *          a(n-k-1).
 * https://oeis.org/A307733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + Σ_{k=0}^{n-1} a(k)*a(n-1-k), a(0) =
 *            a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array holding computed a(n) values */
static size_t cache_size = 0;     /* allocated length of cache */
static idx_t last_computed = 0;   /* highest n that has been computed */

/* Allocate or grow the cache so that it can hold indices up to n inclusive.
   For C we use realloc, for C++ we use new/delete so that any constructors
   of seq_t are invoked. */
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n)
        return;

    size_t new_size = (size_t)(n + 1);
    seq_t *tmp;

#ifdef __cplusplus
    /* In C++ allocate new array and copy the old values */
    tmp = new seq_t[new_size];
    for (size_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];
    delete [] cache;
#else
    /* In C realloc handles copying; we must cast the void* result */
    tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();          /* out of memory – terminate */
#endif

    cache = tmp;
    cache_size = new_size;
}

/* Return the nth term of OEIS A307733.  The routine is memoised: values are
   computed once and reused for any order of calls. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Initialise or expand cache to fit n */
    ensure_cache(n);

    /* In the first call we have to initialize a(0) and a(1) */
    if (last_computed == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* Compute all missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k)
            sum += cache[k] * cache[i - 1 - k];
        cache[i] = cache[i - 1] + cache[i - 2] + sum;
    }
    last_computed = n;

    return cache[n];
}
