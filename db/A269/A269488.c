/*
 * A269488  Number of length-n 0..2 arrays with no repeated value differing
 *          from the previous repeated value by one.
 * https://oeis.org/A269488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) - 4*a(n-3) + 10*a(n-4) + 4*a(n-5).
 *            Initial values: a(1)=3, a(2)=9, a(3)=27, a(4)=77, a(5)=215
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the standard header for memory allocation */
#include <stdlib.h>

/* Compute the nth term of OEIS sequence A269488. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated size in elements */
    static size_t size  = 0;    /* number of computed elements in the cache */

    /* Helper to grow the cache to at least needed elements. */
    if (cap < size)
    {
        /* should not happen but keep safety */
        cap = size;
    }
    if (n > cap)
    {
        size_t newcap = (cap > 0) ? cap * 2 : 16;
        if (newcap < (size_t)n)
            newcap = n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure – unlikely in tests */
        cache = tmp;
        cap = newcap;
    }

    /* Initialize the cache on first use. */
    if (size == 0)
    {
        /* We need at least 5 entries for the recurrence base. */
        size_t initcap = (cap > 0) ? cap : 16;
        cache = (seq_t *)malloc(initcap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure */
        cache[0] = 3;   /* a(1) */
        cache[1] = 9;   /* a(2) */
        cache[2] = 27;  /* a(3) */
        cache[3] = 77;  /* a(4) */
        cache[4] = 215; /* a(5) */
        size = 5;
    }

    /* Return directly if the term has already been computed. */
    if (n <= size)
    {
        return cache[n - 1];
    }

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = size + 1; i <= (size_t)n; ++i)
    {
        seq_t term = 6 * cache[i - 2];
        term = term - 9 * cache[i - 3];
        term = term - 4 * cache[i - 4];
        term = term + 10 * cache[i - 5];
        term = term + 4 * cache[i - 6];
        cache[i - 1] = term;
    }
    size = (size_t)n;
    return cache[n - 1];
}
