/*
 * A318123  a(0) = a(1) = 1; a(n) = Sum_{k=0..n-2} lcm(a(k), a(n-k-2)).
 * https://oeis.org/A318123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; a(n) = Σ_{k=0}^{n-2} lcm(a(k),a(n-k-2))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
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

static inline seq_t lcm_value(seq_t x, seq_t y)
{
    seq_t g = gcd(x, y);
    return (x / g) * y;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap  = 0;

    if (n < 2) {
        /* base values */
        return (seq_t)1;
    }

    /* ensure we have space for indices up to n */
    ensure_cache(n, &cache, &cap);

    /* return already computed value */
    if (cache[n] != 0) {
        return cache[n];
    }

    /* compute sum of lcms, using symmetry to half the work */
    seq_t sum = 0;
    idx_t i = 0;
    idx_t j = n - 2;
    while (i < j) {
        seq_t l = lcm_value(a(i), a(j));
        sum += l;
        sum += l;          /* add twice, for the symmetric term */
        ++i;
        --j;
    }
    if (i == j) {
        /* middle term: lcm(x,x) == x */
        sum += a(i);
    }

    cache[n] = sum;
    return sum;
}
