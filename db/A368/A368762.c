/*
 * A368762  a(n) = n! * (1 + Sum_{k=0..n} binomial(k+1,2) / k!).
 * https://oeis.org/A368762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (1 + Sum_{k=0..n} binomial(k+1,2) / k!); Recurrence:
 *            a(0) = 1; a(n) = n * a(n-1) + binomial(n+1,2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to reuse previous computations, works for any call order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache_len == 0) {               /* first call: initialise cache[0] */
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure fallback */
        cache[0] = 1;                   /* a(0) = 1 */
    }

    if (n >= cache_len) {               /* compute up to the requested n */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return (seq_t)0;      /* allocation failure fallback */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prod = ((seq_t)i) * cache[i-1];            /* i * a(n-1) */
            seq_t add  = ((seq_t)(i+1)) * (seq_t)i / 2;      /* binomial(i+1,2) */
            cache[i] = prod + add;
        }
        cache_len = new_len;
    }

    return cache[n];
}
