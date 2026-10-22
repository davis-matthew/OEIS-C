/*
 * A324152  a(0)=1; for n>0, a(n) = (3/((n+1)*(n+2)*(n+3))) *
 *          multinomial(4*n;n,n,n,n).
 * https://oeis.org/A324152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n)*4*(4*n+1)*(4*n+2)*(4*n+3)/((n+1)^2*(n+4)); a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* Compute A324152: a(n) = 3*(4n)! / ((n!)^3 * (n+3)!)  (for n>0)    */
seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* allocate or grow cache as needed */
    if (n >= cache_size) {
        size_t need = (size_t)n + 1;
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* initialise from the first missing index */
        for (size_t i = cache_size; i < need; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)3;
            } else {
                /* recurrence: a(i) = a(i-1) * 4 * (4*(i-1)+1) * (4*(i-1)+2) * (4*(i-1)+3) /
                                   (i * i * (i+3))                                   */
                idx_t k = i - 1;                     /* previous index          */
                seq_t val = cache[i - 1];

                /* numerator factors */
                seq_t nums[4] = {
                    (seq_t)4,
                    (seq_t)(4 * k + 1),
                    (seq_t)(4 * k + 2),
                    (seq_t)(4 * k + 3)
                };

                /* denominator factors; we track which ones have been cancelled */
                seq_t dens[3] = { (seq_t)i, (seq_t)i, (seq_t)(i + 3) };
                int  done[3]  = { 0, 0, 0 };

                /* multiply by each numerator factor and cancel denominators on the fly */
                for (int j = 0; j < 4; ++j) {
                    val *= nums[j];
                    for (int d = 0; d < 3; ++d) {
                        if (!done[d] && (val % dens[d] == 0)) {
                            val /= dens[d];
                            done[d] = 1;
                        }
                    }
                }

                /* any remaining denominators */
                for (int d = 0; d < 3; ++d) {
                    if (!done[d]) {
                        val /= dens[d];
                        done[d] = 1;
                    }
                }

                cache[i] = val;
            }
        }
        cache_size = need;
    }

    return cache[n];
}
