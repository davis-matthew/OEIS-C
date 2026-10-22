/*
 * A352864  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n-k,k) *
 *          a(n-2*k-1).
 * https://oeis.org/A352864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊(n-1)/2⌋} binomial(n-k,k) * a(n-2k-1) with
 *            initial value a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a binomial utility:
   seq_t binomial(idx_t n, idx_t k);        */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries stored (>=1) */

    /* ensure cache large enough for n */
    if (n >= cache_size) {
        idx_t needed = n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * needed);
        /* compute missing entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            idx_t max_k = (i - 1) / 2;   /* floor((i-1)/2) */
            for (idx_t k = 0; k <= max_k; ++k) {
                seq_t bin = binomial(i - k, k);
                seq_t prod = bin * cache[i - 2 * k - 1];
                sum = sum + prod;
            }
            cache[i] = sum;
        }
        cache_size = needed;
    }

    return cache[n];
}
