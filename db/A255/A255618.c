/*
 * A255618  Number of length n+4 0..2 arrays with at most two downsteps in
 *          every 4 consecutive neighbor pairs.
 * https://oeis.org/A255618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4) - 3*a(n-5) - 8*a(n-6) +
 *            9*a(n-7) - 3*a(n-8) + a(n-9) Initial values for n = 1..9 are
 *            obtained by brute‑force enumeration of the 3^(n+4) arrays and
 *            counting those that satisfy the “at most two downsteps in every
 *            block of 4 consecutive pairs” condition
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   brute_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/brute_n.h"

/* ------------------------------------------------------------ */
/* Convert a raw pointer from realloc to seq_t* when compiling as C++ */
static inline seq_t *realloc_seq(seq_t *ptr, size_t size)
{
#ifdef __cplusplus
    return (seq_t *)realloc(ptr, size);
#else
    return realloc(ptr, size);
#endif
}

/* ------------------------------------------------------------ */
/* Ensure that *ptr holds at least 'needed' seq_t slots.  If a
   realloc with a class type is performed (C++), the old elements are
   copied elementwise to the new array. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr,
                            size_t cur_len)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap) ? (*cap << 1) : 16;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *newptr = NULL;
#ifdef __cplusplus
    newptr = new seq_t[new_cap];
    for (size_t i = 0; i < cur_len; ++i)
        newptr[i] = (*ptr)[i];
    delete[] *ptr;
#else
    newptr = realloc_seq(*ptr, new_cap * sizeof(seq_t));
#endif
    *ptr = newptr;
    *cap = new_cap;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Apply the empirical recurrence */
static inline seq_t rec(const seq_t *c, idx_t k)
{
    /* k ≥ 10 when this function is called */
    return 3 * c[k - 2]                 /* a(n-1) */
         - c[k - 4]                     /* a(n-3) */
         + 3 * c[k - 5]                 /* a(n-4) */
         - 3 * c[k - 6]                 /* a(n-5) */
         - 8 * c[k - 7]                 /* a(n-6) */
         + 9 * c[k - 8]                 /* a(n-7) */
         - 3 * c[k - 9]                 /* a(n-8) */
         + c[k - 10];                   /* a(n-9) */
}

/* ------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined below offset, return 0 */

    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* number of computed terms */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* allocate space for up to n terms */
    ensure_capacity((size_t)n, &cache_cap, &cache, cache_size);

    /* initialise first nine terms if needed */
    if (cache_size < 9) {
        for (idx_t k = cache_size + 1; k <= 9; ++k)
            cache[k - 1] = brute_n(k);
        cache_size = 9;
    }

    /* compute further terms using recurrence */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        cache[k - 1] = rec(cache, k);
    }
    cache_size = (size_t)n;

    return cache[n - 1];
}
