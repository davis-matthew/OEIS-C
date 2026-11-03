/*
 * A332239  a(n) = n^(n-2) - Sum_{k=1..n-1} k^(k-2) * a(n-k).
 * https://oeis.org/A332239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2) - Sum_{k=1..n-1} k^(k-2) * a(n-k); a(1)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_idx.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t *a_cache = NULL;
static size_t a_cap = 0;
static idx_t a_len = 0;

static seq_t *pow_cache = NULL;
static size_t pow_cap = 0;
static idx_t pow_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0; /* Offset starts at 1; return 0 for safety. */

    /* Ensure caches large enough. */
    ensure_cache(n, &a_cache, &a_cap);
    ensure_cache(n, &pow_cache, &pow_cap);

    /* Initialize base case if not done yet. */
    if (a_len == 0) {
        a_cache[1] = 1;
        pow_cache[1] = 1;
        a_len = 1;
        pow_len = 1;
    }

    /* Compute terms up to n if needed. */
    while (a_len < n) {
        idx_t m = a_len + 1;

        /* Compute n^(n-2).  For m>=2, exponent is non‑negative. */
        seq_t term = ipow_idx((seq_t)m, (idx_t)(m - 2));
        pow_cache[m] = term;           /* Store k^(k-2) for future use. */

        seq_t sum = 0;
        for (idx_t k = 1; k < m; ++k) {
            seq_t prod = seq_mul(pow_cache[k], a_cache[m - k]);
            sum += prod;
        }

        a_cache[m] = term - sum;       /* a(m) = n^(n-2) - Σ k^(k-2)*a(n-k) */
        a_len = m;
        if (pow_len < m) pow_len = m;
    }

    return a_cache[n];
}
