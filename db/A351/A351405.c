/*
 * A351405  a(1) = 1; a(n+1) = Sum_{d|n} 2^(n/d - 1) * a(d).
 * https://oeis.org/A351405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = Σ_{d|n} 2^(n/d - 1) * a(d)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t computed = 0;

static void ensure_cache_size(idx_t n)
{
    if (n + 1 <= cap) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failed; let caller handle */
    if (cache) {
        memcpy(new_cache, cache, cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, return 0 */

    if (n > computed) {
        ensure_cache_size(n);
        /* initialize base case */
        if (computed == 0) {
            cache[1] = 1;
            computed = 1;
        }
        for (idx_t i = computed + 1; i <= n; ++i) {
            idx_t m = i - 1;           /* we need divisors of m */
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    /* first divisor d */
                    idx_t exp1 = (m / d) - 1;
                    seq_t term1 = pow2(exp1) * cache[d];
                    sum += term1;
                    /* paired divisor */
                    idx_t d2 = m / d;
                    if (d2 != d) {
                        idx_t exp2 = (m / d2) - 1;
                        seq_t term2 = pow2(exp2) * cache[d2];
                        sum += term2;
                    }
                }
            }
            cache[i] = sum;
            computed = i;
        }
    }
    return cache[n];
}
