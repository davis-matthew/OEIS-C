/*
 * A355294  Expansion of e.g.f. 1 / (1 - x - x^2/2 - x^3/3 - x^4/4).
 * https://oeis.org/A355294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + n*(n-1)*a(n-2)/2 + n*(n-1)*(n-2)*a(n-3)/3 * +
 *            n*(n-1)*(n-2)*(n-3)*a(n-4)/4; Initial values: * a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=14
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_cap  = 0;      /* allocated capacity (n+1) */
static idx_t  max_computed = 0;    /* highest index computed so far */

seq_t a(idx_t n)
{
    /* Allocate initial cache if not yet done */
    if (!cache) {
        size_t init_cap = 4;                     /* enough for a(0..3) */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failed */
        cache_cap = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)14;
        max_computed = 3;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= max_computed) return cache[(size_t)n];

    /* Compute terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term = (seq_t)i * cache[(size_t)(i-1)];
        term += ((seq_t)i * (seq_t)(i-1) * cache[(size_t)(i-2)]) / (seq_t)2;
        term += ((seq_t)i * (seq_t)(i-1) * (seq_t)(i-2) * cache[(size_t)(i-3)])
               / (seq_t)3;
        term += ((seq_t)i * (seq_t)(i-1) * (seq_t)(i-2) *
                 (seq_t)(i-3) * cache[(size_t)(i-4)]) / (seq_t)4;
        cache[(size_t)i] = term;
    }

    max_computed = n;
    return cache[(size_t)n];
}
