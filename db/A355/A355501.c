/*
 * A355501  Expansion of e.g.f. exp(3 * x * exp(x)).
 * https://oeis.org/A355501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 3 * Σ_{k=1..n} k * binomial(n-1, k-1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation note:  the recurrence uses only binomial and integer
 *  arithmetic.  A static array is used to memoize values so that
 *  subsequent calls, even for arbitrary ordering of n, run in O(n²)
 *  time and O(n) memory.  The helper binomial() is provided by
 *  "../utils/binomial.h".  No other external utilities are required.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t  cache_size = 0;    /* allocated size of cache */
    static idx_t  computed   = 0;    /* next index to compute (so far, values
                                      * 0 .. computed-1 are valid)  */

    /* allocate or grow cache if needed */
    if (cache == NULL) {
        cache = (seq_t*)calloc(n + 1, sizeof(seq_t));
        cache_size = n + 1;
        computed   = 0;             /* nothing computed yet */
    } else if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) {          /* allocation failure – return 0 */
            return (seq_t)0;
        }
        cache   = tmp;
        cache_size = n + 1;
        /* new slots are left zeroed by realloc; they will be
         * overwritten when computed. */
    }

    /* initial value a(0) = 1 */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;           /* next to compute is index 1 */
    }

    /* compute missing entries up to n */
    for (idx_t m = computed; m <= n; ++m)
    {
        seq_t sum = 0;          /* Σ_{k=1..m} k * binomial(m-1,k-1) * a(m-k) */

        for (idx_t k = 1; k <= m; ++k)
        {
            seq_t t = binomial(m - 1, k - 1);          /* C(m-1, k-1)   */
            t *= cache[m - k];                        /* * a(m-k)       */
            t *= (seq_t)k;                            /* * k            */
            sum += t;                                 /* accumulate */
        }

        cache[m] = sum * ((seq_t)3);                  /* final factor 3 */
    }

    computed = n + 1;      /* all indices up to n are now computed */
    return cache[n];
}
