/*
 * A281204  Number of n X 7 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-4) + 4*a(n-5) - 2*a(n-6) +
 *            4*a(n-7) - a(n-8) - 2*a(n-10) - a(n-12) with initial values a(1)
 *            = 20, a(2) = 166, …, a(12) = 438532. a(0) is unused. This
 *            recurrence is implemented below with memoisation so that any
 *            order of calls is supported
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 26, u64 n <= 64, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, size_t */

static seq_t *cache = NULL;            /* dynamic array of computed terms   */
static idx_t  cache_size = 0;          /* allocated size (number of entries)*/
static idx_t  max_computed = 0;        /* highest n for which cache[n] is valid */

static const seq_t init_vals[13] = {
    (seq_t)0,          /* unused, a(0) */ 
    (seq_t)20,         /* a(1) */
    (seq_t)166,        /* a(2) */
    (seq_t)537,        /* a(3) */
    (seq_t)1358,       /* a(4) */
    (seq_t)3137,       /* a(5) */
    (seq_t)6838,       /* a(6) */
    (seq_t)14375,      /* a(7) */
    (seq_t)29416,      /* a(8) */
    (seq_t)58984,      /* a(9) */
    (seq_t)116466,     /* a(10) */
    (seq_t)227134,     /* a(11) */
    (seq_t)438532      /* a(12) */
};

seq_t a(idx_t n)
{
    /* Initialise cache on the first call */
    if (!cache) {
        cache_size = 16;                     /* start with room for 16 entries */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* out of memory -> 0 as fallback */

        /* Fill the known initial values */
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init_vals[i];
        max_computed = 12;
    }

    /* Return if already computed */
    if (n <= max_computed)
        return cache[n];

    /* Grow cache if necessary; ensure capacity > n */
    if (cache_size <= n) {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* out of memory -> 0 */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[i] = init_vals[i];         /* safety, should not hit */
            continue;
        }
        seq_t val = 4 * cache[i - 1];
        val -= 4 * cache[i - 2];
        val -= 2 * cache[i - 4];
        val += 4 * cache[i - 5];
        val -= 2 * cache[i - 6];
        val += 4 * cache[i - 7];
        val -=  cache[i - 8];
        val -= 2 * cache[i -10];
        val -=  cache[i -12];
        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
