/*
 * A355233  E.g.f. A(x) satisfies A'(x) = 1 + 2 * (exp(x) - 1) * A(x).
 * https://oeis.org/A355233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 0
 *   a(1) = 1
 *   a(n+1) = 2 * sum_{k=1}^{n-1} binomial(n,k) * a(k)   (n >= 1)
 *
 * The implementation uses memoisation; previously returned values are kept
 * so that calls are efficient in any order.  The cache is grown as needed.
 * For large n the sequence becomes huge – the code uses only the integer
 * operations permitted for seq_t and does not rely on any floating‑point.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* highest index already computed */
    static size_t cache_cap   = 0;   /* allocated cache size              */

    /* need room for indices 0..n inclusive; at least 2 positions for a(0),a(1) */
    size_t needed = (size_t)n + 1;
    if (needed < 2) needed = 2;

    if (cache == NULL) {
        cache_cap = needed;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure – unlikely */
        cache[0] = 0;
        cache[1] = 1;
        max_computed = 1;
    } else if (needed > cache_cap) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    if (n <= max_computed) return cache[n];

    /* compute terms from max_computed+1 to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t value;
        if (i == 0) {
            value = 0;
        } else if (i == 1) {
            value = 1;
        } else {
            /* sum_{k=1}^{i-2} binomial(i-1,k) * a(k) */
            seq_t sum = 0;
            idx_t m = i - 1;                 /* n = i-1 in the recurrence */
            seq_t bin = (seq_t)m;            /* binomial(m,1) */
            for (idx_t k = 1; k <= i - 2; ++k) {
                sum += bin * cache[k];
                seq_t num = bin * (seq_t)(m - k);
                bin = num / (seq_t)(k + 1);  /* next binomial value */
            }
            value = sum * 2;
        }
        cache[i] = value;
    }
    max_computed = n;
    return cache[n];
}
