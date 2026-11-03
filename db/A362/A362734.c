/*
 * A362734  E.g.f. satisfies A(x) = exp(x + x * A(x)^3).
 * https://oeis.org/A362734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0}^{n} (3*k+1)^{\,n-1} * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 314 (2026-11-03)
 * Helpers:   binomial, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, so we handle it explicitly.  The function uses the
   provided utilities for binomial coefficients and exponentiation.  */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
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

static seq_t *cache = NULL;          /* cache of computed values          */
static size_t cache_cap = 0;         /* current capacity of 'cache'       */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* Ensure the cache is large enough for index n */
    ensure_cache(n, &cache, &cache_cap);
    if (cache[n] != (seq_t)0)             /* already computed?               */
        return cache[n];

    seq_t sum = 0;   /* accumulator for the sum */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = (seq_t)(3 * (seq_t)k + (seq_t)1);   /* 3*k+1   */
        seq_t power = ipow(base, (seq_t)(n - 1));     /* (3k+1)^{n-1} */
        seq_t comb  = binomial(n, k);                   /* C(n,k)   */
        seq_t term  = comb * power;                     /* term product */
        sum += term;                                    /* accumulate */
    }

    cache[n] = sum;   /* store result for future calls */
    return sum;
}
