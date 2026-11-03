/*
 * A247328  Odd deficient numbers.
 * https://oeis.org/A247328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = the nth odd deficient number. Computed by iterating over
 *            odd integers and keeping those with sigma(n) < 2n
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 769 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety for offset 1 */

    static seq_t *cache      = NULL;      /* cached terms */
    static size_t cache_cap  = 0;          /* capacity of cache */
    static size_t cache_len  = 0;          /* number of cached terms */
    static idx_t  next_odd   = 1;          /* next odd integer to test */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate terms until we have the requested index */
    while (cache_len < (size_t)n) {
        idx_t m = next_odd;
        next_odd += 2;                     /* advance to next odd */

        seq_t sigma = sigma_of(m);
        seq_t twice = ((seq_t)m) * ((seq_t)2);

        if (sigma < twice) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
