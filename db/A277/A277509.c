/*
 * A277509  Expansion of e.g.f. 1/((1+LambertW(-x))*(1+x)).
 * https://oeis.org/A277509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = n^n - n*a(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small, seq_alloc, seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size of cache */
    static idx_t  computed = 0;   /* number of terms actually computed   */

    /* grow cache to hold up to index n */
    if (n >= computed) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL) {
            cache = seq_alloc(new_cap);
        } else if (new_cap > capacity) {
            cache = seq_realloc(cache, capacity, new_cap);
        }
        capacity = new_cap;

        /* initialise the first term if needed */
        if (computed == 0) {
            cache[0] = 1;        /* a(0) = 1 */
            computed = 1;
        }

        /* compute missing terms from computed up to n      */
        for (idx_t i = computed; i <= n; ++i) {
            seq_t term = seq_pow((seq_t)i, i);          /* i^i   */
            seq_t prod = mul_small(cache[i-1], (unsigned int)i); /* n*a(n-1) */
            cache[i]   = term - prod;                  /* a(n)  */
        }
        computed = n + 1;
    }

    return cache[n];
}
