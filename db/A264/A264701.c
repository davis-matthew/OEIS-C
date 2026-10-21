/*
 * A264701  Number of n X 1 arrays of permutations of 0..n-1 with rows
 *          nondecreasing modulo 2 and columns nondecreasing modulo 6.
 * https://oeis.org/A264701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0}^{5} floor((n+i)/6)!
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 25, u64 n <= 38, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#if defined(__cplusplus)
#  include <new>
#  define ALLOC(count, type) (new type[count])
#  define FREE(p)            (delete[] (p))
#else
#  include <stdlib.h>
#  define ALLOC(count, type) ((type*)malloc((count) * sizeof(type)))
#  define FREE(p)            free(p)
#endif

seq_t a(idx_t n)
{
    /* cache of integer factorials up to the largest needed value */
    static seq_t *fact_cache = NULL;
    static idx_t fact_size = 0;     /* number of precomputed factorials */

    /* determine the maximal floor((n+i)/6) needed */
    idx_t max_t = 0;
    for (idx_t i = 0; i < 6; ++i)
    {
        idx_t t = (n + i) / 6;
        if (t > max_t)
            max_t = t;
    }

    /* grow the factorial cache if necessary */
    if (fact_size <= max_t)
    {
        idx_t new_size = max_t + 1;          /* need index up to max_t */
        seq_t *new_cache = ALLOC(new_size, seq_t);

        /* copy existing values */
        for (idx_t j = 0; j < fact_size; ++j)
            new_cache[j] = fact_cache[j];

        /* initialize missing entries */
        if (fact_size == 0)
        {
            new_cache[0] = 1;   /* 0! = 1 */
            fact_size = 1;
        }

        for (idx_t j = fact_size; j < new_size; ++j)
        {
            new_cache[j] = new_cache[j - 1] * (seq_t)j;
        }

        /* free old cache */
        if (fact_cache)
            FREE(fact_cache);

        fact_cache = new_cache;
        fact_size = new_size;
    }

    /* compute the product of the six factorials */
    seq_t result = 1;
    for (idx_t i = 0; i < 6; ++i)
    {
        idx_t t = (n + i) / 6;
        result *= fact_cache[t];
    }

    return result;
}
