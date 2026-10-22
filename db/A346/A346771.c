/*
 * A346771  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - x)) / (1 - x^2).
 * https://oeis.org/A346771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} binomial(n-1,k) * a(n-k-1), with a(0) =
 *            a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache for indices 0 and 1 */
    if (cache == NULL) {
        cache_size = 2;                     /* indices 0 and 1 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;               /* memory failure */
        cache[0] = 1;
        cache[1] = 1;
    }

    /* grow cache to include requested n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                 /* memory failure */
        cache = tmp;

        /* compute terms from old_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = 1;
                continue;
            }
            seq_t sum = 0;
            seq_t c = 1;                     /* C(i-1,0) */
            for (idx_t k = 1; k <= i - 1; ++k) {
                /* update c to C(i-1,k) */
                c = (c * ((seq_t)(i - 1) - (seq_t)(k - 1))) / (seq_t)k;
                sum += c * cache[i - k - 1];
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
