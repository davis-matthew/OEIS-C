/*
 * A333589  a(n) = floor(a(n-1)*3/2) bitwise-OR LSB(a(n-1)*n), with a(1)=2
 *          (LSB = Least Significant Bit).
 * https://oeis.org/A333589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(a(n‑1)*3/2) bitwise‑OR LSB(a(n‑1)*n), a(1)=2
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence can be simplified to:   */
/*   let x = a(n‑1)            */
/*   base = x + (x >> 1)      (i.e. floor(3*x/2))   */
/*   if n is odd, a(n) = base | (x & 1)               */
/*   else          a(n) = base                         */

#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"
#include <stddef.h>

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* outside offset, unused but harmless */

    /* Expand cache if necessary */
    if ((idx_t)n > (idx_t)cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = alloc_cache(new_cap);
        /* Copy old data if any */
        for (size_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = seq_cache[i];
        }
        if (seq_cache) {
            free_cache(seq_cache);
        }
        seq_cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise first term if not done yet */
    if (cache_len == 0) {
        seq_cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* Return already computed value */
    if ((idx_t)n <= (idx_t)cache_len) {
        return seq_cache[(size_t)n - 1];
    }

    /* Compute remaining terms up to n */
    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
        seq_t prev = seq_cache[(size_t)i - 2];
        seq_t val = prev + (prev >> 1);      /* floor(3*prev/2) */
        if (i & 1U) {                        /* n odd: OR with LSB(prev) */
            val |= (prev & (seq_t)1);
        }
        seq_cache[(size_t)i - 1] = val;
    }
    cache_len = (size_t)n;
    return seq_cache[(size_t)n - 1];
}
