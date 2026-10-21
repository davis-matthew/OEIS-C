/*
 * A247328  Odd deficient numbers.
 * https://oeis.org/A247328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = the nth odd deficient number. Computed by iterating over
 *            odd integers and keeping those with sigma(n) < 2n
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety for offset 1 */

    static seq_t *cache      = NULL;      /* cached terms */
    static size_t cache_cap  = 0;          /* capacity of cache */
    static size_t cache_len  = 0;          /* number of cached terms */
    static idx_t  next_odd   = 1;          /* next odd integer to test */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
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
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
