/*
 * A393535  G.f. A(x) satisfies: A(x) = 1 + x * A(2*x/(1 - 2*x)) / (1 -
 *          2*x)^2.
 * https://oeis.org/A393535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2^(n-1) * Σ_{k=0}^{n-1} binomial(n,k+1) * a(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;   /* last computed index */

    /* ensure memory for [0..n] */
    ensure_cache(n, &cache, &cap);

    /* base term */
    if (n == 0)
        return 1;
    if (cache[0] == 0)
        cache[0] = 1;

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = i;                 /* C(i,1) */

        for (idx_t k = 0; k < i; ++k) {
            seq_t prod = cache[k] * binom;
            sum += prod;

            if (k + 1 < i) {
                seq_t num = (seq_t)(i - (k + 1));
                seq_t den = (seq_t)(k + 2);
                binom = (binom * num) / den;    /* C(i,k+2) */
            }
        }

        cache[i] = sum * pow2(i - 1);
    }

    max_computed = n;
    return cache[n];
}
