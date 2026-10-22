/*
 * A394940  Number of well-formed bracketed words of total length n built from
 *          the symbol * (of length 1) and three unary bracket types, with no
 *          empty bracket pair.
 * https://oeis.org/A394940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=1; for n>=3: a(n)=a(n-1)+3*a(n-2)+3*Σ_{i=1}^{n-3}
 *            a(i)*a(n-2-i)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 38, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;

    if (n < cache_sz) /* already computed */
        return cache[n];

    /* enlarge cache */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {
        cache = NULL;
        cache_sz = 0;
        return (seq_t)0; /* allocation failure */
    }
    cache = new_cache;

    /* compute new entries */
    for (idx_t i = cache_sz; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i - 3; ++k) {
                sum += cache[k] * cache[i - 2 - k];
            }
            cache[i] = cache[i - 1] + ((seq_t)3 * cache[i - 2]) + ((seq_t)3 * sum);
        }
    }
    cache_sz = n + 1;
    return cache[n];
}
