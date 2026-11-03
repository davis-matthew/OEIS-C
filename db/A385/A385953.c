/*
 * A385953  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(k+4,4) * a(k) * a(n-1-k).
 * https://oeis.org/A385953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A385953
 * Recurrence: a(0) = 1
 *             a(n) = Σ_{k=0}^{n-1} binomial(k+4,4) * a(k) * a(n-1-k)   (n ≥ 1)
 */

#include <stddef.h>          /* for size_t */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* memoisation array */
    static size_t cache_cap  = 0;      /* allocated size of cache */
    static idx_t computed_max = 0;     /* highest n already computed */

    /* Ensure the cache is large enough to hold a[n] */
    if ((size_t)n >= cache_cap) {
        grow_seq(&cache, &cache_cap, (size_t)n + 1);
    }

    /* Special handling for n == 0 */
    if (n == 0) {
        if (computed_max == 0) {          /* first time we see a(0) */
            cache[0] = (seq_t)1;
            computed_max = 0;
        }
        return cache[0];
    }

    /* Initialise a(0) on the first use */
    if (computed_max == 0) {
        cache[0] = (seq_t)1;
        computed_max = 0;
    }

    /* Compute missing values up to n (if any) */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t binom = binomial((idx_t)k + 4, 4);
                seq_t prod  = cache[k] * cache[i - 1 - k];
                sum += binom * prod;
            }
            cache[i] = sum;
        }
        computed_max = n;
    }

    return cache[n];
}
