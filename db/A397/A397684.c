/*
 * A397684  Number of n-digit numbers such that, if the digit j occurs, then
 *          every digit from 0 to j also occurs.
 * https://oeis.org/A397684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..9} k*k!*StirlingS2(n,k+1) (Stirling numbers of
 *            the second kind)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute a(n) for n >= 1
 * a(0) is treated as 0 (offset 1 in OEIS). */
seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;          /* array of a(i) for i=1..cache_size */
    static idx_t cache_size = 0;         /* number of terms stored */
    static int initialized = 0;

    /* static Stirling number accumulator: st[k] = S(current_n, k) for k=0..10 */
    static seq_t st[11];
    static int st_init = 0;
    static seq_t fact[11];
    static int fact_init = 0;

    if (!initialized) {
        /* no terms computed yet */
        cache = NULL;
        cache_size = 0;
        initialized = 1;
    }

    if (n == 0) return (seq_t)0;   /* behaviour below offset is unused */

    /* initialise factorial table once */
    if (!fact_init) {
        fact[0] = (seq_t)1;
        for (unsigned k = 1; k <= 10; ++k)
            fact[k] = fact[k - 1] * (seq_t)k;
        fact_init = 1;
    }

    /* initialise Stirling array once */
    if (!st_init) {
        st[0] = (seq_t)1;    /* S(0,0) = 1 */
        for (unsigned k = 1; k <= 10; ++k)
            st[k] = (seq_t)0;
        st_init = 1;
    }

    /* extend cache if necessary */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) abort();   /* out of memory */
        cache = new_cache;

        /* compute new terms from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* update Stirling numbers to S(i, k) */
            for (int k = 10; k >= 1; --k)
                st[k] = (seq_t)k * st[k] + st[k - 1];
            st[0] = (seq_t)0;

            /* accumulate a(i) */
            seq_t ai = (seq_t)0;
            for (int k = 1; k <= 9; ++k) {
                seq_t term = st[k + 1];
                term *= fact[k];          /* multiply by k! */
                term *= (seq_t)k;         /* multiply by k */
                ai += term;
            }
            cache[i - 1] = ai;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
