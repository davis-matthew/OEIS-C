/*
 * A277378  Expansion of e.g.f. exp(2*x/(1-x))/sqrt(1-x^2).
 * https://oeis.org/A277378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |H_n(i)|^2 / 2^n = H_n(i) * H_n(-i) / 2^n Recurrence:
 *            (n+2)*(a(n)+n*a(n-1)) = a(n+1)+n*(n-1)^2*a(n-2) Initial values:
 *            a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* number of terms stored */

static void ensure_cache(idx_t n) {
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        /* Allocate or grow the cache */
        if (cache == NULL)
            cache = seq_alloc(needed);
        else
            cache = seq_realloc(cache, cache_cap, needed);

        /* Compute missing terms starting from the previous capacity */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i == 1) {
                cache[i] = 2;
            } else {
                seq_t n_seq = (seq_t)(i - 1);            /* n = i-1   */
                seq_t a_n = cache[i - 1];               /* a(n)     */
                seq_t a_n_m1 = cache[i - 2];            /* a(n-1)   */
                seq_t a_n_m2 = cache[i - 3];            /* a(n-2)   */

                seq_t left   = (n_seq + (seq_t)2) *
                               (a_n + n_seq * a_n_m1);
                seq_t n_minus1 = n_seq - (seq_t)1;
                seq_t term3 = n_seq * n_minus1 * n_minus1 * a_n_m2;

                cache[i] = left - term3;
            }
        }
        cache_cap = needed;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
