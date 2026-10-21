/*
 * A257939  x-values in the solutions to x^2 + x = 5*y^2 + y.
 * https://oeis.org/A257939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257939 : x-values in the solutions to x^2 + x = 5*y^2 + y.
   Recurrence for n > 5:
     a(n) = a(n-1) + 322*a(n-2) - 322*a(n-3) - a(n-4) + a(n-5)
   Initial values:
     a(1)=0,  a(2)=2,  a(3)=116,  a(4)=798,  a(5)=37512. */

#include <stdlib.h>   /* malloc, realloc */

/* Cached sequence values.  Static so that calls in any order succeed. */
static seq_t *seq_cache = NULL;
static idx_t  seq_cache_size = 0;   /* Number of terms stored (>=0) */

seq_t a(idx_t n)
{
    /* Initialise cache with the first five terms on first call. */
    if (seq_cache_size == 0) {
        seq_cache_size = 5;
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * seq_cache_size);
        if (!seq_cache) return (seq_t)0;   /* allocation failure, return 0 */

        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)2;
        seq_cache[2] = (seq_t)116;
        seq_cache[3] = (seq_t)798;
        seq_cache[4] = (seq_t)37512;
    }

    /* Compute up to index n (1‑based).  Cache indices are 0‑based. */
    while (seq_cache_size < n) {
        idx_t k = seq_cache_size + 1; /* next 1‑based index to compute */

        /* a(k) = a(k-1) + 322*a(k-2) - 322*a(k-3) - a(k-4) + a(k-5) */
        seq_t val =  seq_cache[k-2]                     /* a(k-1) */
                  + (seq_t)322 * seq_cache[k-3]         /* + 322*a(k-2) */
                  - (seq_t)322 * seq_cache[k-4]         /* - 322*a(k-3) */
                  - seq_cache[k-5]                     /* - a(k-4)   */
                  + seq_cache[k-6];                    /* + a(k-5)   */

        /* Grow the cache by one element. */
        seq_t *new_cache = (seq_t *)realloc(seq_cache,
                                            sizeof(seq_t) * (seq_cache_size + 1));
        if (!new_cache) { /* allocation failure – return current value */
            return seq_cache[seq_cache_size - 1];
        }
        seq_cache = new_cache;
        seq_cache[seq_cache_size] = val;
        seq_cache_size++;   /* now seq_cache_size == k */
    }

    return seq_cache[n - 1];
}
