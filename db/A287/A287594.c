/*
 * A287594  Number of independent vertex sets in the n-helm graph.
 * https://oeis.org/A287594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + A080040(n) = 2^n + (1-√3)^n + (1+√3)^n Recurrence:
 *            a(0)=3, a(1)=4, a(2)=12, a(n)=4*a(n-1)-2*a(n-2)-4*a(n-3) (n≥3)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed values                */
static idx_t  cache_cap = 0;     /* current number of allocated slots      */
static idx_t  max_computed = 2;  /* largest index that has been filled     */

/* Ensure the cache can hold indices up to n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = n + 1;
    seq_t *tmp = NULL;

#ifdef __cplusplus
    /* Allocate a new array with default construction. */
    tmp = new seq_t[new_cap];
    /* Copy existing values if any. */
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            tmp[i] = cache[i];
        delete[] cache;
    }
#else
    /* Reallocate with memory copy semantics suitable for integral types. */
    tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize cache with the first three terms. */
static void
init_cache(void)
{
    if (cache != NULL)
        return;

    ensure_capacity(2);
    cache[0] = (seq_t)3;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)12;
    max_computed = 2;
}

/* Return the n‑th term of the sequence. */
seq_t
a(idx_t n)
{
    init_cache();

    /* Grow the cache and compute any terms up to n. */
    if (n > max_computed) {
        if (n + 1 > cache_cap)
            ensure_capacity(n + 1);

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t term = cache[i - 1] * (seq_t)4;
            term -= cache[i - 2] * (seq_t)2;
            term -= cache[i - 3] * (seq_t)4;
            cache[i] = term;
        }
        max_computed = n;
    }

    return cache[n];
}
