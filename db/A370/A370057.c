/*
 * A370057  a(n) = 3*(4*n+2)!/(3*n+3)!.
 * https://oeis.org/A370057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(4*n+2)!/(3*n+3)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence: 3*(3*n+2)*(3*n+1)*(n+1)*a(n) =
 *               8*n*(4*n+1)*(2*n+1)*(4*n-1)*a(n-1)  ,  a(0)=1  */
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *seq_cache = NULL;   /* cache of already computed terms   */
static idx_t seq_cache_size = 0;  /* number of elements stored in cache */

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* Ensure cache[0] = 1 (a(0)) */
    if (seq_cache_size == 0) {
        seq_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!seq_cache) return 0;                 /* out of memory; treat as 0   */
        seq_cache[0] = (seq_t)1;
        seq_cache_size = 1;
    }

    /* Expand cache up to needed index */
    if (n + 1 > seq_cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* out of memory; treat as 0   */
        seq_cache = tmp;
        /* Compute missing terms */
        for (idx_t i = seq_cache_size; i <= n; ++i) {
            /* Previous term a(i-1) */
            seq_t result = seq_cache[i - 1];

            /* Convert i to seq_t for arithmetic */
            seq_t sn = (seq_t)i;

            /* Numerator factors: 8, n, 4n+1, 2n+1, 4n-1 */
            seq_t num[5];
            num[0] = (seq_t)8;
            num[1] = sn;
            num[2] = (seq_t)4 * sn + 1;
            num[3] = (seq_t)2 * sn + 1;
            num[4] = (seq_t)4 * sn - 1;

            /* Denominator factors: 3, 3n+2, 3n+1, n+1 */
            seq_t den[4];
            den[0] = (seq_t)3;
            den[1] = (seq_t)3 * sn + 2;
            den[2] = (seq_t)3 * sn + 1;
            den[3] = sn + 1;

            /* Cancel common factors between result and denominators */
            for (int j = 0; j < 4; ++j) {
                seq_t g = gcd(result, den[j]);
                if (g > 1) {
                    result /= g;
                    den[j] /= g;
                }
            }

            /* Iterate over numerator factors, cancel with remaining denominators,
               then multiply into result                                    */
            for (int k = 0; k < 5; ++k) {
                seq_t f = num[k];
                for (int j = 0; j < 4; ++j) {
                    seq_t g = gcd(f, den[j]);
                    if (g > 1) {
                        f /= g;
                        den[j] /= g;
                    }
                }
                result *= f;
            }

            /* After processing, all den[*] should be 1 */
            seq_cache[i] = result;
        }
        seq_cache_size = new_cap;
    }

    return seq_cache[n];
}
