/*
 * A346052  G.f. A(x) satisfies: A(x) = 1 + x + x^3 * A(x/(1 - x)) / (1 - x).
 * https://oeis.org/A346052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 22, u64 n <= 35, u128 n <= 57,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 642 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A346052
   Formula: a(0) = a(1) = 1, a(2) = 0,
   a(n) = Sum_{k=0..n-3} binomial(n-3,k) * a(k)   for n >= 3
   (OEIS offset: 0)
   Implementation: dynamic programming with memoisation.
   The recurrence is evaluated by computing the binomial
   coefficients incrementally to avoid large intermediate
   factorials.  The function is order‑independent and
   remains efficient for N up to about 10 000. */

#include <stdlib.h>
#include <stddef.h>
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
#include <stddef.h>                  // for size_t

extern seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoisation buffer */
    static size_t cap = 0;
    static idx_t computed_max = (idx_t)-1;  /* largest index already computed */

    /* Ensure the cache can hold indices up to n    */
    ensure_cache(n, &cache, &cap);

    /* Initialise base cases on first use or if n < 3   */
    if (computed_max == (idx_t)-1) {
        if (n >= 0) cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)1;
        if (n >= 2) cache[2] = (seq_t)0;
        computed_max = (n >= 2) ? (idx_t)2 : (idx_t)n;
    }

    /* Compute any missing values up to index n   */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i == 0) { cache[i] = (seq_t)1; continue; }
            if (i == 1) { cache[i] = (seq_t)1; continue; }
            if (i == 2) { cache[i] = (seq_t)0; continue; }

            /* recurrence for i >= 3 */
            idx_t m = i - 3;                /* upper binomial index     */
            seq_t bin = (seq_t)1;           /* C(m,0) = 1 */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= m; ++k) {
                sum += bin * cache[k];
                if (k == m) break;
                /* update binomial coefficient: C(m,k+1) = C(m,k) * (m-k)/(k+1) */
                bin = bin * (seq_t)(m - k) / (seq_t)(k + 1);
            }
            cache[i] = sum;
        }
        computed_max = n;
    }

    return cache[n];
}
