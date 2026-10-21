/*
 * A265051  Poincaré series for hyperbolic reflection group with Coxeter
 *          diagram shown in Comments.
 * https://oeis.org/A265051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coeff of x^n in (1+x)^3*(1-x+x^2)*(1+x^2) / /
 *            ((1-x)*(1-x-x^2-x^4-x^5+x^6))
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 35, u64 n <= 73, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* coefficients of the numerator polynomial N(x) */
static const seq_t num_coeff[8] = {1, 2, 2, 3, 3, 2, 2, 1};

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* Out of memory */
        cache = new_cache;

        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t val = 0;
            if (i < 8)          /* c_i = num_coeff[i] for i = 0..7, else 0 */
                val = num_coeff[i];

            if (i >= 1) val += 2 * cache[i - 1];
            if (i >= 3) val -= cache[i - 3];
            if (i >= 4) val += cache[i - 4];
            if (i >= 6) val -= 2 * cache[i - 6];
            if (i >= 7) val += cache[i - 7];

            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[idx];
}
