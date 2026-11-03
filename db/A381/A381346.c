/*
 * A381346  Expansion of e.g.f. 1/( 1 - sinh(sqrt(2)*x) / sqrt(2) ).
 * https://oeis.org/A381346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} 2^k * binomial(n, 2*k+1) *
 *            a(n-2*k-1) with a(0)=1, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   binomial, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
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
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    ensure_cache(n, &cache, &cache_cap);

    if (cache[n] != 0)
        return cache[n];

    seq_t res;
    if (n == 0 || n == 1)
        res = 1;
    else {
        res = 0;
        idx_t kmax = (n - 1) / 2;
        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t term = binomial(n, 2 * k + 1);
            term = seq_mul(term, pow2(k));
            term = seq_mul(term, a(n - 2 * k - 1));
            res += term;
        }
    }

    cache[n] = res;
    return res;
}
