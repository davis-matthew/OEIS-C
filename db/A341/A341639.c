/*
 * A341639  a(1) = 1; a(n+1) = Sum_{d|n} phi(d) * a(d) * a(n/d).
 * https://oeis.org/A341639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A341639 sequence
    Formula:  a(1) = 1,
              a(n+1) = Sum_{d|n} φ(d) · a(d) · a(n/d)   (n ≥ 1)
*/

#include <stdlib.h>
#include <stddef.h>
#include "../utils/calc_phi.h"

static seq_t *cache = NULL;     /* a(i) stored at cache[i-1]    */
static size_t cache_len = 0;    /* number of terms computed so far */
static size_t cache_cap = 0;    /* allocated size of cache         */

/* Grow cache to contain at least `n` terms (n ≥ 1) */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;

    if (cache_len == 0) {                  /* initialise a(1) */
        size_t init_cap = (n < 2) ? 2 : (size_t)n;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = init_cap;
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if (n <= cache_len)
        return;

    if (n > cache_cap) {                  /* enlarge storage */
        size_t new_cap = (cache_cap > 0) ? cache_cap << 1 : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t m = cache_len + 1; m <= n; ++m) {
        idx_t ndiv = m - 1;               /* n in the recurrence */
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= ndiv; ++d) {
            if (ndiv % d == 0) {
                idx_t q = ndiv / d;
                seq_t phi_d = calc_phi((seq_t)d);
                seq_t term1 = phi_d * cache[d - 1] * cache[q - 1];
                sum += term1;
                if (d != q) {
                    seq_t phi_q = calc_phi((seq_t)q);
                    seq_t term2 = phi_q * cache[q - 1] * cache[d - 1];
                    sum += term2;
                }
            }
        }
        cache[m - 1] = sum;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n = 0, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
