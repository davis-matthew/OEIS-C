/*
 * A341693  Numbers k whose sum of digits divides sigma(k)-k.
 * https://oeis.org/A341693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_sum, ensure_capacity, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A341693: numbers k whose sum of digits divides sigma(k)-k.
   a(n) = the nth such k (indexed from 1). */

#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/digit_sum.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* Static cache of sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t next_k = 1;        /* next integer to test */

    /* Grow cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t sigma = sigma_of(next_k);
        seq_t dsum = digit_sum((seq_t)next_k);
        /* sigma - k is always >=0 for k>=1 */
        seq_t diff = sigma - (seq_t)next_k;
        if (diff % dsum == 0) {
            /* add to cache */
            if (cache_len >= cache_cap) {
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;  /* advance to next integer */
    }

    return cache[n - 1];
}
