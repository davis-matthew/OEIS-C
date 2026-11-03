/*
 * A340887  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n,k)^2 * 3^(n-k-1) *
 *          a(k).
 * https://oeis.org/A340887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   binomial, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A340887: a(0)=1, a(n)=sum_{k=0}^{n-1} binomial(n,k)^2 * 3^(n-k-1) * a(k)  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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

    /* Ensure cache large enough for indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Base case */
    if (n == 0)
    {
        cache[0] = 1;
        return 1;
    }

    /* Return cached value if already computed */
    if (cache[n] != 0)
        return cache[n];

    /* pre‑compute 3^(n-1) and then repeatedly divide by 3 */
    seq_t pow3 = ipow((seq_t)3, n - 1);

    seq_t sum = 0;
    seq_t binom = 1;              /* C(n,0) */

    for (idx_t k = 0; k < n; ++k)
    {
        /* term = binom^2 * 3^(n-k-1) * a(k) */
        seq_t term = seq_mul(binom, binom);       /* binom^2   */
        term = seq_mul(term, pow3);              /* * 3^(n-k-1) */
        term = seq_mul(term, cache[k]);          /* * a(k)    */

        sum += term;                             /* accumulate */

        /* update pow3 for next iteration (divide by 3) */
        pow3 = pow3 / (seq_t)3;

        /* update binomial for next k: C(n,k+1) = C(n,k)*(n-k)/(k+1) */
        binom = seq_mul(binom, (seq_t)(n - k));
        binom = binom / (k + 1);
    }

    cache[n] = sum;
    return sum;
}
