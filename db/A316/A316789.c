/*
 * A316789  Number of same-tree-factorizations of n.
 * https://oeis.org/A316789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{n = x^y, y > 1} a(x)^y
 * Range:     u8 n <= 4095, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8292 (2026-11-03)
 * Helpers:   ilog2, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>               /* size_t */
#include <stdint.h>               /* uint64_t */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ilog2.h"
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

/* Return n^(1/y) rounded down.
   Uses safe multiplication to avoid overflow.                  */
static uint64_t power_limit(uint64_t base, uint64_t exp, uint64_t limit)
{
    uint64_t res = 1;
    for (uint64_t i = 0; i < exp; ++i) {
        if (res > limit / base) return limit + 1;   /* overflow, exceeds limit */
        res *= base;
    }
    return res;
}

static uint64_t int_nth_root(uint64_t n, uint64_t y)
{
    uint64_t low = 1, high = n;
    uint64_t best = 1;

    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t p = power_limit(mid, y, n);
        if (p > n) {
            if (mid == 0) break;
            high = mid - 1;
        } else {
            best = mid;
            low = mid + 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* Memoization cache.  cache[1] corresponds to a(1). */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    ensure_cache(n, &cache, &cap);          /* guarantees cache[0..n] non‑null, zeroed */

    if (cache[n] != 0) return cache[n];     /* already computed */

    if (n == 1) {                           /* base case */
        cache[1] = 1;
        return 1;
    }

    seq_t sum = 0;
    idx_t max_exp = ilog2(n);   /* y > 1, y ≤ log₂(n) */
    for (idx_t y = 2; y <= max_exp; ++y) {
        uint64_t r = int_nth_root((uint64_t)n, (uint64_t)y);
        if (r <= 1) continue;              /* r must be > 1 */

        /* Verify r^y == n using safe multiplication */
        uint64_t pp = power_limit(r, (uint64_t)y, (uint64_t)n);
        if (pp != (uint64_t)n) continue;   /* not an exact y‑th power */

        seq_t a_r = a(r);                  /* recursive value for a(r)   */
        seq_t pow_val = ipow(a_r, (seq_t)y);
        sum += pow_val;
    }

    cache[n] = 1 + sum;                     /* a(n) = 1 + sum            */
    return cache[n];
}
