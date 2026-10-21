/*
 * A289843  p-INVERT of (1,0,2,0,3,0,4,0,5,...) (A027656), where p(S) = 1 - S
 *          - S^2.
 * https://oeis.org/A289843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289843 (OEIS)
   Formula (recurrence):
       a(n) = a(n-1) + 5*a(n-2) - 2*a(n-3) - 6*a(n-4) + a(n-5) + 4*a(n-6) - a(n-8)
   Initial values:
       a(0)=1, a(1)=2, a(2)=5, a(3)=13, a(4)=29, a(5)=73, a(6)=168, a(7)=410
   a(8) and onward are defined by the recurrence above.
*/

#include <stdlib.h>   /* malloc, realloc */

/* Static cache for memoisation; indices 0..next_computed-1 are valid */
static seq_t *cache = NULL;      /* array of computed a(i) */
static size_t cache_size = 0;    /* allocated capacity */
static size_t next_computed = 0; /* first index that has NOT yet been computed */

/* Base values for indices 0..7 */
static const seq_t base_vals[8] = {
    (seq_t)1,  (seq_t)2,  (seq_t)5,  (seq_t)13, (seq_t)29,
    (seq_t)73, (seq_t)168, (seq_t)410
};

/* Compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n < 8)
        return base_vals[(size_t)n];

    /* Ensure the cache is large enough */
    size_t need = (size_t)n + 1;
    if (need > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* allocation failure – unlikely for tested sizes */
        cache = tmp;
        cache_size = need;
    }

    /* Initialise base values in the cache on first use */
    if (next_computed == 0) {
        for (size_t i = 0; i < 8; ++i)
            cache[i] = base_vals[i];
        next_computed = 8;
    }

    /* Return already computed value if requested */
    if ((size_t)n < next_computed)
        return cache[(size_t)n];

    /* Compute all missing terms up to n */
    for (size_t i = next_computed; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1];
        val += (seq_t)5 * cache[i-2];
        val -= (seq_t)2 * cache[i-3];
        val -= (seq_t)6 * cache[i-4];
        val +=            cache[i-5];
        val += (seq_t)4 * cache[i-6];
        val -=            cache[i-8];
        cache[i] = val;
    }

    next_computed = (size_t)n + 1;
    return cache[(size_t)n];
}
