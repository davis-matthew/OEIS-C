/*
 * A291740  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = (1 - S)(1 - S^2).
 * https://oeis.org/A291740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+2*a(n-4)-3*a(n-5)+a(n-6)-3*a(n-7)-a(n-9) *
 *            for n ≥ 10 with initial values * a(0)=1, a(1)=2, a(2)=3, a(3)=7,
 *            a(4)=9, * a(5)=18, a(6)=25, a(7)=47, a(8)=65, a(9)=118
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 50, u64 n <= 107,
 *            u128 n <= 221, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* hard‑coded first ten values */
    static const seq_t init[10] = {
        (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)7, (seq_t)9,
        (seq_t)18, (seq_t)25, (seq_t)47, (seq_t)65, (seq_t)118
    };

    /* trivial case */
    if (n < (idx_t)10) return init[n];

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;   /* current allocated size      */
    static idx_t cache_computed = 0;   /* next index to compute (size) */

    /* enlarge cache if necessary */
    if (cache_capacity <= n) {
        idx_t new_cap = n + 1;
        if (new_cap < 32) new_cap = 32;           /* avoid too small reallocs   */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                       /* unrecoverable allocation    */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* initialise first ten entries if not already done */
    if (cache_computed < (idx_t)10) {
        for (idx_t i = 0; i < (idx_t)10; ++i) {
            cache[i] = init[i];
        }
        cache_computed = (idx_t)10;
    }

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = cache_computed; i <= n; ++i) {
        seq_t val = cache[i-1] + cache[i-2];
        val += (seq_t)2 * cache[i-4];
        val -= (seq_t)3 * cache[i-5];
        val += cache[i-6];
        val -= (seq_t)3 * cache[i-7];
        val -= cache[i-9];
        cache[i] = val;
    }

    cache_computed = n + 1;   /* all indices < n are now computed */
    return cache[n];
}
