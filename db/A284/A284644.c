/*
 * A284644  a(1) = a(2) = 2, a(3) = 1; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for n
 *          > 3.
 * https://oeis.org/A284644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 412, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A284644
    Recurrence:  a(1) = a(2) = 2, a(3) = 1;
                  a(n) = a(n-a(n-1)) + a(n-a(n-2))   for n > 3
    The function below memoises the sequence up to the largest n
    requested, so calls can be made in any order.  All arithmetic is
    performed in the type seq_t; indices use idx_t (unsigned 64‑bit).  */
#include <stdlib.h>
#include <string.h>

static seq_t   *cache   = NULL;   /* 1‑based storage, cache[0] unused */
static size_t   cap     = 0;      /* allocated size (in elements)   */
static idx_t    max_computed = 0; /* largest n that has been computed */

/* Expand cache so it can hold indices up to 'needed' (inclusive).  */
static void grow_cache(size_t needed)
{
    size_t new_cap = cap ? cap << 1 : 4;
    while (new_cap <= needed) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();           /* out of memory */
    if (cache) memcpy(new_cache, cache,
                      (max_computed + 1) * sizeof(seq_t));

    free(cache);
    cache = new_cache;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<=0, avoid UB */

    /* Ensure cache can address index n.  */
    if (!cache || (size_t)n + 1 > cap)
        grow_cache((size_t)n + 1);

    /* Initialise base values on first use.  */
    if (max_computed == 0) {
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
        max_computed = 3;
    }

    if (n <= max_computed) return cache[(size_t)n];

    /* Compute all missing values up to n.  */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i-a(i-1)) + a(i-a(i-2)) */
        idx_t idx1 = i - (idx_t)cache[i-1];
        idx_t idx2 = i - (idx_t)cache[i-2];
        seq_t v1 = cache[(size_t)idx1];
        seq_t v2 = cache[(size_t)idx2];
        cache[(size_t)i] = v1 + v2;
    }
    max_computed = n;
    return cache[(size_t)n];
}
