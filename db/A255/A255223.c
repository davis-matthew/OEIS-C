/*
 * A255223  Number of (n+2)X(3+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 0 and no antidiagonal sum 0 and no row sum 2 and no column sum
 *          2.
 * https://oeis.org/A255223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255223: a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4)
   with a(1)=31, a(2)=37, a(3)=46, a(4)=58. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; for n<1 the value is undefined, return 0. */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Handle the first four terms explicitly (no array needed). */
    if (n <= (idx_t)4) {
        static const seq_t init[4] = {
            (seq_t)31, (seq_t)37, (seq_t)46, (seq_t)58
        };
        return init[n - 1];
    }

    /* Memoise computed values in a static dynamic array.  The array
       stores a(i) in cache[i-1] (i.e. cache[0] = a(1)).  */
    static seq_t *cache = NULL;    /* pointer to array of seq_t */
    static size_t cache_len = 0;   /* number of elements currently stored */

    /* Initialise cache with the first four terms if this is the first call. */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL)
            return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)31;
        cache[1] = (seq_t)37;
        cache[2] = (seq_t)46;
        cache[3] = (seq_t)58;
    }

    /* If the requested n is already cached, return it immediately. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Ensure we can cast n to size_t safely for allocation. */
    if (n > (idx_t)SIZE_MAX)
        return (seq_t)0;   /* too large to handle */

    /* Grow the cache to accommodate up to n. */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL)
        return (seq_t)0;   /* allocation failure */
    cache = new_cache;

    /* Compute new terms using the recurrence.  For i >= 4
       cache[i] corresponds to a(i+1). */
    for (size_t i = cache_len; i < new_len; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)2;   /* 2*a(i)   */
        seq_t t2 = t1 - cache[i - 2];         /* -a(i-1)  */
        seq_t t3 = t2 + cache[i - 3];         /* +a(i-2)  */
        cache[i] = t3 - cache[i - 4];         /* -a(i-3)  */
    }

    cache_len = new_len;
    return cache[n - 1];
}
