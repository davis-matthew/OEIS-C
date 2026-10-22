/*
 * A307875  G.f. A(x) satisfies: A(x) = 1 + x*(1 + x*A(x/(1 - x)^2)/(1 -
 *          x)^2).
 * https://oeis.org/A307875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A307875
   Recurrence:
      a(0) = 1, a(1) = 1
      a(n+1) = Σ_{k=0..n} binomial(n+k, 2*k+1) * a(k)
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed values */
    static size_t cap   = 0;         /* current allocation size */
    static idx_t  max   = 0;         /* highest index cached */

    /* Handle n = 0 directly */
    if (n == 0)
        return (seq_t)1;

    /* Initialisation on first call */
    if (cache == NULL)
    {
        /* Allocate space for at least two entries: a(0) and a(1) */
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            abort(); /* allocation failed */
        cache[0] = (seq_t)1; /* a(0) */
        cache[1] = (seq_t)1; /* a(1) */
        max = 1;
    }

    /* If the requested value is already cached, return it */
    if (n <= max)
        return cache[n];

    /* Ensure enough space for indices up to n */
    if ((size_t)(n + 1) > cap)
    {
        size_t new_cap = cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort(); /* reallocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute values from max+1 up to n */
    for (idx_t i = max + 1; i <= n; ++i)
    {
        /* recurrence valid for i >= 2 (a(0) and a(1) already set) */
        if (i < 2)
        {
            cache[i] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k)
        {
            seq_t coeff = binomial((i - 1) + k, 2 * k + 1);
            sum += coeff * cache[k];
        }
        cache[i] = sum;
    }

    max = n;
    return cache[n];
}
