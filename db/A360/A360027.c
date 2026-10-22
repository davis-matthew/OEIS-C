/*
 * A360027  a(n) = Sum_{k=0..floor(n/5)} (-1)^k * binomial(n-4*k,k) *
 *          Catalan(k).
 * https://oeis.org/A360027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 - Σ_{k=0}^{n-5} a(k) * a(n-5-k)
 * Range:     i8 n <= 19, i16 n <= 37, i32 n <= 64, i64 n <= 121,
 *            i128 n <= 233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with initial values a(0)=a(1)=a(2)=a(3)=a(4)=1          */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;               /* allocation failure – undefined but safe */
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 5) {
                cache[i] = (seq_t)1;
            } else {
                /* compute a(i) = 1 - Σ_{k=0}^{i-5} a(k) * a(i-5-k) */
                seq_t sum = (seq_t)0;
                idx_t limit = i - 5;
                for (idx_t k = 0; k <= limit; ++k) {
                    sum += cache[k] * cache[limit - k];
                }
                cache[i] = (seq_t)1 - sum;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
