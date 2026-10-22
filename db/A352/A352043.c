/*
 * A352043  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/4)} binomial(n-3*k-1,k) *
 *          a(k).
 * https://oeis.org/A352043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 ; a(n) = ∑_{k=0..⌊(n-1)/4⌋} binomial(n-3k-1,k) * a(k)
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 64, u64 n <= 124,
 *            u128 n <= 240, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;      /* array of a(i) values */
    static idx_t cache_cap = 0;      /* allocated size */
    static idx_t cache_last = 0;     /* highest index currently computed */

    /* first time: allocate cache[0] = a(0) = 1 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;/* out‑of‑memory: caller must tolerate 0 */
        cache[0] = 1;
        cache_last = 0;
    }

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;/* out‑of‑memory: caller must tolerate 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n if not yet computed */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        idx_t maxk = (i - 1) / 4;
        seq_t sum = 0;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t bin = binomial(i - 3 * k - 1, k);
            sum += bin * cache[k];
        }
        cache[i] = sum;
    }
    cache_last = n;

    return cache[n];
}
