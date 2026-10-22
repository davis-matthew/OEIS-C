/*
 * A370847  a(n) = (n-1)*n!*(n-1)!.
 * https://oeis.org/A370847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n!*(n-1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case (n < 2) */
    if (n < 2)
        return (seq_t)0;

    /* static cache for factorials */
    static seq_t *fact_cache = NULL;
    static size_t fact_cache_cap = 0;   /* maximum factorial computed so far */

    /* Lazy initialization of cache (factorial 0! = 1) */
    if (fact_cache == NULL) {
        fact_cache = (seq_t*)malloc(1 * sizeof(seq_t));   /* for index 0 */
        fact_cache[0] = (seq_t)1;     /* 0! = 1 */
        fact_cache_cap = 0;
    }

    /* Grow cache up to n if necessary */
    if ((size_t)n > fact_cache_cap) {
        size_t old_cap = fact_cache_cap;
        size_t new_cap = (size_t)n;   /* we need up to index n */
        seq_t *tmp = (seq_t*)realloc(fact_cache, (new_cap + 1) * sizeof(seq_t));
        fact_cache = tmp;
        for (size_t i = old_cap + 1; i <= new_cap; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_cap = new_cap;
    }

    idx_t m = n - 1;
    /* (n-1) * n! * (n-1)! */
    seq_t result = (seq_t)m * fact_cache[n] * fact_cache[m];
    return result;
}
