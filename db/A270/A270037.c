/*
 * A270037  a(n) = Smallest m >= 3 containing no zeros when represented in any
 *          base from 3 through n.
 * https://oeis.org/A270037
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer m ≥ 3 such that m has no zero digit in
 *            base b for all b in [3, n]. The function computes the sequence
 *            value for each n ≥ 3, caching results for efficiency
 * Range:     u8 n <= 156, u16 n <= 618, u32 n <= 618, u64 n <= 618,
 *            u128 n <= 618, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Checks whether integer m (in seq_t) has no digit 0 in any base from 3 up to n. */
static inline bool ok(seq_t m, idx_t n)
{
    for (idx_t base = 3; base <= n; ++base) {
        seq_t bb  = (seq_t)base;
        seq_t tmp = m;
        while (tmp > 0) {
            if ((tmp % bb) == 0) {
                return false;
            }
            tmp /= bb;
        }
    }
    return true;
}

/* External function returning a(n) for n ≥ 3. */
seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;          /* undefined for n < 3 in OEIS */
    }

    /* Static workspace for memoisation. */
    static seq_t *cache      = NULL;  /* cache[i] holds a(i) for i ≤ size-1 */
    static idx_t cache_size  = 0;      /* current allocated size of cache   */
    static idx_t last_n      = 2;      /* largest n for which cache is filled */
    static seq_t current_m   = 3;      /* candidate m for next computation  */

    /* Grow cache if necessary. */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation error */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = 0;
        }
        cache_size = n + 1;
    }

    /* Compute a(k) for successive k from last_n+1 up to n. */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        seq_t m = current_m;                 /* start search from previous result */
        while (!ok(m, k)) {
            ++m;
        }
        cache[k] = m;
        current_m = m;                       /* next search can start from here */
    }

    last_n = n;
    return cache[n];
}
