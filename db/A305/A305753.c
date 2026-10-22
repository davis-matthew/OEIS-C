/*
 * A305753  A base-3/2 sorted Fibonacci sequence that starts with a(0) = 0 and
 *          a(1) = 1. The terms are interpreted as numbers written in base
 *          3/2. To get a(n+2), add a(n) and a(n+1), write the result in base
 *          3/2 and sort the "digits" into increasing order, omitting all
 *          zeros.
 * https://oeis.org/A305753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + 10*a(n-2) - 10*a(n-3) for n>4, with a(0)=0,
 *            a(1)=a(2)=1, a(3)=a(4)=2
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;     /* array of computed terms */
static idx_t cache_cap = 0;     /* current number of stored terms */

/* Ensure that cache has at least 'required' elements.  New elements are
   filled using the recurrence relation. */
static void ensure_cache(idx_t required)
{
    if (required <= cache_cap)
        return;

    seq_t *new_cache;
#ifdef __cplusplus
    /* Allocate and construct objects automatically (C++). */
    new_cache = new seq_t[required];
#else
    /* Allocate raw storage (C). */
    new_cache = (seq_t *)malloc((size_t)required * sizeof(seq_t));
    if (!new_cache)
        abort(); /* out‑of‑memory */
#endif

    /* Copy existing values. */
    for (idx_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    /* Compute missing terms. */
    for (idx_t i = cache_cap; i < required; ++i)
    {
        if (i == 0)          new_cache[i] = (seq_t)0;
        else if (i == 1 ||
                 i == 2)     new_cache[i] = (seq_t)1;
        else if (i == 3 ||
                 i == 4)     new_cache[i] = (seq_t)2;
        else
            new_cache[i] = new_cache[i-1] + (seq_t)10 * new_cache[i-2]
                         - (seq_t)10 * new_cache[i-3];
    }

    /* Release old storage. */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_cache;
    cache_cap = required;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n + 1);
    return cache[n];
}
