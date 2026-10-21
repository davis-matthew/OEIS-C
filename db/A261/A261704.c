/*
 * A261704  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000101 or 00000111.
 * https://oeis.org/A261704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)+a(n-3)+5*a(n-4)-3*a(n-5)-5*a(n-6)+3*a(n-7)-4*a(n-8)+2*a(n-9)
 *            for n>11. Initial values: a(1)=48, a(2)=76, a(3)=172, a(4)=338,
 *            a(5)=628, a(6)=1298, a(7)=2752, a(8)=5526, a(9)=10972,
 *            a(10)=22462, a(11)=46160
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Computes A261704; works for any seq_t / idx_t provided by the build. */
seq_t a(idx_t n)
{
    /* If n<1, return 0. */
    if (n < 1)
        return (seq_t)0;

    /* A static dynamic cache for memoisation. */
    static seq_t *cache      = NULL;   /* array of values, 1‑based */
    static size_t cache_cap  = 0;      /* allocated capacity (≥ n+1)   */
    static idx_t cache_last  = 0;      /* highest index already computed */

    /* Ensure the cache can hold indexes up to n. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)     /* allocation failure – give up */
            return (seq_t)0;
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first 11 values lazily. */
    if (cache_last < 11) {
        static const seq_t init[11] = {
            (seq_t)48, (seq_t)76, (seq_t)172, (seq_t)338, (seq_t)628,
            (seq_t)1298, (seq_t)2752, (seq_t)5526, (seq_t)10972,
            (seq_t)22462, (seq_t)46160
        };
        idx_t up_to = (n < 11 ? n : 11);
        for (idx_t i = cache_last + 1; i <= up_to; ++i)
            cache[i] = init[i - 1];
        cache_last = up_to;
    }

    /* Compute further values using the recurrence. */
    if (n > 11 && cache_last < n) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t val = 0;
            val += cache[i - 1];
            val += cache[i - 2];
            val += cache[i - 3];
            val += (seq_t)5 * cache[i - 4];
            val -= (seq_t)3 * cache[i - 5];
            val -= (seq_t)5 * cache[i - 6];
            val += (seq_t)3 * cache[i - 7];
            val -= (seq_t)4 * cache[i - 8];
            val += (seq_t)2 * cache[i - 9];
            cache[i] = val;
        }
        cache_last = n;
    }

    return cache[n];
}
