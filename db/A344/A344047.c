/*
 * A344047  a(n) = Sum_{d|n} sigma(d)^d.
 * https://oeis.org/A344047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma(d)^d
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t *cache = NULL;
static size_t cache_size = 0;  /* next index to be computed (cache[0] unused) */

/* Ensure that values a(1) .. a(up_to) are cached. */
static void ensure_cache(idx_t up_to)
{
    if (cache_size > up_to) return;

    size_t new_cap = up_to + 1;
    seq_t *new_cache = mem_new(seq_t, new_cap);
    if (!new_cache) return;  /* allocation failed – leave cache unchanged */

    /* copy existing cached values */
    for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];

    /* initialise new slots */
    for (size_t i = cache_size; i < new_cap; ++i) new_cache[i] = (seq_t)0;

    /* free old cache */
    mem_free(cache);
    cache = new_cache;

    /* compute missing values */
    for (idx_t m = cache_size; m <= up_to; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                seq_t sig_d = sigma_of(d);
                sum += ipow_idx(sig_d, d);
                idx_t d2 = m / d;
                if (d2 != d) {
                    seq_t sig_d2 = sigma_of(d2);
                    sum += ipow_idx(sig_d2, d2);
                }
            }
        }
        cache[m] = sum;
    }

    cache_size = up_to + 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
