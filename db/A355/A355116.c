/*
 * A355116  a(1) = 1; a(n+1) = Sum_{d|n} 3^(n/d - 1) * a(d).
 * https://oeis.org/A355116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355116:  a(1) = 1;  a(n+1) = Σ_{d|n} 3^(n/d-1) * a(d)  (n ≥ 1) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow3.h"   /* seq_t pow3(idx_t exp); */

static seq_t *cache = NULL;       /* cache[1..] stores computed terms */
static idx_t cache_size = 0;      /* current size of cache */

/* Ensure cache can hold at least indices up to up_to (inclusive) and
   compute any missing values.  Assumes cache is initially empty. */
static void ensure_cache(idx_t up_to)
{
    if (up_to <= cache_size)
        return;   /* already computed */

    /* Grow array if necessary */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)calloc(cache_size + 1, sizeof(seq_t));
        if (!cache) abort();
        cache[1] = 1;              /* a(1) = 1 */
    } else if (up_to > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (up_to + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* No need to init new elements; they'll be filled below */
    }

    /* Compute terms in increasing order */
    for (idx_t k = cache_size + 1; k <= up_to; ++k) {
        idx_t m = k - 1;           /* m = n-1 for a(n) */
        seq_t sum = 0;
        idx_t i = 1;
        for (; i * i <= m; ++i) {
            if (m % i == 0) {
                idx_t d1 = i;
                idx_t exp1 = m / d1 - 1;
                sum += cache[d1] * pow3(exp1);

                idx_t d2 = m / i;
                if (d2 != d1) {
                    idx_t exp2 = m / d2 - 1;
                    sum += cache[d2] * pow3(exp2);
                }
            }
        }
        cache[k] = sum;
    }
    cache_size = up_to;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* Out of bounds; the problem guarantees n ≥ 1 */
    ensure_cache(n);
    return cache[n];
}
