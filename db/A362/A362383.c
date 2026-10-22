/*
 * A362383  Number of labeled right involutory magmas with n elements.
 * https://oeis.org/A362383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000085(n)^n (number of labeled right involutory magmas)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *invol_cache = NULL;
static idx_t invol_cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure that involution numbers up to n are computed and cached. */
    if (n + 1 > invol_cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache;
        if (invol_cache == NULL) {
            new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(invol_cache, new_size * sizeof(seq_t));
        }
        if (!new_cache) {
            /* Allocation failed – return 0 as a graceful failure. */
            return 0;
        }
        invol_cache = new_cache;
        idx_t old_size = invol_cache_size;
        invol_cache_size = new_size;

        /* Compute involution numbers I(i) = I(i-1) + (i-1)*I(i-2). */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i == 0) {
                invol_cache[0] = 1;
            } else if (i == 1) {
                invol_cache[1] = 1;
            } else {
                invol_cache[i] = invol_cache[i - 1]
                                 + (seq_t)(i - 1) * invol_cache[i - 2];
            }
        }
    }

    /* a(n) = I(n)^n.  Use the provided big_pow routine. */
    seq_t invol = invol_cache[n];
    seq_t result = big_pow(invol, (seq_t)n);
    return result;
}
