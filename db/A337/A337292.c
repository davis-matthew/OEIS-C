/*
 * A337292  a(n) = 4*binomial(5*n,n)/(5*n-1).
 * https://oeis.org/A337292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*binomial(5*n,n)/(5*n-1) /* Recurrence used: a(1)=5; for
 *            n>=2 a(n) = 5*(5n-4)*(5n-3)*(5n-2)*(5n-6) /
 *            (8n*(4n-3)*(2n-1)*(4n-1)) * a(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *seq_cache = NULL;   /* cache of computed values */
static size_t seq_cap = 0;        /* allocated capacity of cache */
static idx_t last_computed = 0;   /* highest n computed so far */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset, but harmless */

    /* Ensure cache can hold up to n */
    if ((size_t)n >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;            /* allocation failure */
        seq_cache = new_cache;
        seq_cap   = new_cap;
    }

    /* Compute terms up to n if needed */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i == 1) {
                seq_cache[1] = 5;
            } else {
                seq_t res = seq_cache[i - 1];

                /* Numerator factors: 5, (5i-4), (5i-3), (5i-2), (5i-6) */
                idx_t num[5] = {5,
                                5 * i - 4,
                                5 * i - 3,
                                5 * i - 2,
                                5 * i - 6};

                /* Denominator factors: 8i, (4i-3), (2i-1), (4i-1) */
                seq_t den[4];
                den[0] = 8 * i;
                den[1] = 4 * i - 3;
                den[2] = 2 * i - 1;
                den[3] = 4 * i - 1;

                for (int k = 0; k < 5; ++k) {
                    res *= (seq_t)num[k];
                    for (int j = 0; j < 4; ++j) {
                        while (den[j] != 1) {
                            seq_t g = gcd(res, den[j]);
                            if (g == 1) break;
                            res /= g;
                            den[j] /= g;
                        }
                    }
                }

                /* Final cancellation to clean any remaining factors */
                for (int j = 0; j < 4; ++j) {
                    while (den[j] != 1) {
                        seq_t g = gcd(res, den[j]);
                        if (g == 1) break;
                        res /= g;
                        den[j] /= g;
                    }
                }

                seq_cache[i] = res;
            }
        }
        last_computed = n;
    }

    return seq_cache[n];
}
