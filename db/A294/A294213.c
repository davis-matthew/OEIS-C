/*
 * A294213  E.g.f.: exp(1/((1-x)*(1-x^2)) - 1).
 * https://oeis.org/A294213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=5, a(3)=25, a(4)=193 a(n) = n*a(n-1) +
 *            (n-1)*(2*n-1)*a(n-2) - 2*(n-3)*(n-2)*(n-1)*a(n-3) -
 *            (n-4)*(n-3)*(n-2)*(n-1)*a(n-4) +
 *            (n-5)*(n-4)*(n-3)*(n-2)*(n-1)*a(n-5) for n ≥ 5
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc/realloc/free */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* dynamic array of computed terms   */
    static size_t  cap   = 0;      /* allocated size (in elements)      */
    static idx_t   last  = 4;      /* index of the largest precomputed term */

    /* initialise the cache and the first five terms on the first call */
    if (cache == NULL) {
        /* allocate space for at least five elements */
        cap   = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory, return 0 as a safe value */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)193;
    }

    /* if the requested index is within what we already have, return it */
    if (n <= last)
        return cache[n];

    /* ensure we have enough space for all terms up to n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* out‑of‑memory, return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms one by one */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = 0;

        /* use explicit casts to seq_t for each index component */
        term += (seq_t)i * cache[i - 1];
        term += (seq_t)(i - 1) * (seq_t)(2 * i - 1) * cache[i - 2];
        term -= (seq_t)2 * (seq_t)(i - 3) * (seq_t)(i - 2) *
                (seq_t)(i - 1) * cache[i - 3];
        term -= (seq_t)(i - 4) * (seq_t)(i - 3) * (seq_t)(i - 2) *
                (seq_t)(i - 1) * cache[i - 4];
        term += (seq_t)(i - 5) * (seq_t)(i - 4) * (seq_t)(i - 3) *
                (seq_t)(i - 2) * (seq_t)(i - 1) * cache[i - 5];

        cache[i] = term;
    }
    last = n;
    return cache[n];
}
