/*
 * A279046  a(n) = A000178(n) * Sum_{k=0..n} (-1)^k/k!.
 * https://oeis.org/A279046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000178(n) * Sum_{k=0..n} (-1)^k/k!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 1 */
/*  a(n) = n! * a(n-1) + (-1)^n * (n-1)! * s(n-2) */
/*  where s(m) = ∏_{k=1}^m k!  (the super‑factorial, with s(0)=1). */

/* This function keeps a cache of previously computed values and
   extends it only when a larger index is requested.  The values are
   stored as seq_t, which may be an arbitrary‑precision integer type. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache and auxiliary state */
    static seq_t *cache      = NULL;   /* a[0] .. a[cache_len-1]           */
    static idx_t cache_len   = 0;      /* number of cached entries          */
    static seq_t fact        = 1;      /* (cache_len)!                      */
    static seq_t superfact   = 1;      /* s(cache_len-1) = product_{k=1}^{cache_len-1} k!   */

    /* Initialise the cache on the first call */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) { return (seq_t)0; }   /* failure: return 0 (unlikely) */
        cache[0] = (seq_t)1;               /* a(0) = 1                         */
        cache_len = 1;
        fact = (seq_t)1;                   /* 0! = 1                            */
        superfact = (seq_t)1;              /* s(0) = 1                           */
    }

    /* Grow the cache up to the requested n */
    while (cache_len <= n) {
        idx_t i = cache_len;                 /* new index to compute             */
        seq_t fact_prev = fact;          /* (i-1)!   */
        seq_t super_old = superfact;     /* s(i-2)   */
        seq_t a_prev = cache[i-1];       /* a(i-1)   */

        /* A_{i-1} = (i-1)! * s(i-2) */
        seq_t A_prev = fact_prev * super_old;

        /* n! * a(n-1)  = fact_prev * i * a_prev */
        seq_t temp = fact_prev * (seq_t)i * a_prev;

        /* Add or subtract A_prev depending on parity of i */
        if ((i & 1) == 0) {
            temp += A_prev;              /* even i: (+) */
        } else {
            temp -= A_prev;              /* odd i:  (-) */
        }

        /* Store the computed value */
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (!cache) { return (seq_t)0; }  /* failure: return 0 */
        cache[cache_len] = temp;

        /* Update state for the next iteration */
        /* superfact becomes s(i-1) = s(i-2) * (i-1)!   */
        superfact *= fact_prev;
        /* fact becomes i!   */
        fact *= (seq_t)i;

        cache_len++;
    }

    return cache[n];
}
