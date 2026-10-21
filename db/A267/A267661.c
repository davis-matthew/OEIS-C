/*
 * A267661  Number of nX2 0..1 arrays with every repeated value in every row
 *          unequal to the previous repeated value, and in every column equal
 *          to the previous repeated value, and new values introduced in
 *          row-major sequential order.
 * https://oeis.org/A267661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 6*a(n-3) + 4*a(n-4) + 2*a(n-5) -
 *            a(n-6); a(1)=2, a(2)=8, a(3)=32, a(4)=98, a(5)=288, a(6)=800.
 *            OEIS A267661
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Static cache of computed values                                               */
/* cache[i] holds a(i); index 0 is unused.                                       */
static seq_t *cache = NULL;       /* pointer to array of cache elements      */
static size_t cache_cap = 0;      /* current capacity (>= largest index   +1)*/
static idx_t  max_computed = 0;   /* largest n with cache[n] valid           */

/* Ensure that cache can store indices up to n (inclusive)                      */
static void ensure_cache(idx_t n)
{
    if (n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;

#ifdef __cplusplus
        /* Allocate new array and copy existing entries.                          */
        seq_t *tmp = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                tmp[i] = cache[i];
            delete[] cache;
        }
#else
        /* Plain realloc works for integral seq_t types and for most big int    */
        /* classes that support trivial allocation.                              */
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the nth term of the sequence (n ≥ 1).                                  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_cache(n);

    /* Initialise first six values once.                                        */
    if (max_computed == 0) {
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)32;
        cache[4] = (seq_t)98;
        cache[5] = (seq_t)288;
        cache[6] = (seq_t)800;
        max_computed = 6;
    }

    /* Already computed?                                                          */
    if (n <= max_computed)
        return cache[n];

    /* Compute remaining terms using the recurrence.                             */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Sum of positive coefficients                                 */
        seq_t pos = cache[i-1] * (seq_t)4;
        pos += cache[i-4] * (seq_t)4;
        pos += cache[i-5] * (seq_t)2;

        /* Sum of negative coefficients                                 */
        seq_t neg = cache[i-2] * (seq_t)2;
        neg += cache[i-3] * (seq_t)6;
        neg += cache[i-6];

        cache[i] = pos - neg;
    }

    max_computed = n;
    return cache[n];
}
