/*
 * A376097  a(0) = 1; a(n) = Sum_{k=0..n-1} (k+1)^4 * a(k) * a(n-k-1).
 * https://oeis.org/A376097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} (k+1)^4 * a(k) * a(n-k-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated size of cache */
    static idx_t last = 0;         /* last index computed (a[0] is already set) */

    /* initialize cache if first call */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return (seq_t)0;      /* out of memory; caller should handle */
        cache[0] = (seq_t)1;                    /* a(0) = 1 */
        last = 0;
    }

    /* grow cache if necessary */
    if (n >= cap) {
        idx_t new_cap = cap ? cap : 1;
        while (new_cap <= n) {
            new_cap <<= 1;
        }
        seq_t *p = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (p == NULL) return (seq_t)0;         /* realloc failed */
        cache = p;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t k1 = (seq_t)(k + 1);
            seq_t w = k1 * k1 * k1 * k1;    /* (k+1)^4 */
            sum += w * cache[k] * cache[i - 1 - k];
        }
        cache[i] = sum;
    }
    last = (idx_t) n;
    return cache[n];
}
