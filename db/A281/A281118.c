/*
 * A281118  a(1)=1, a(n>1) = number of tree-factorizations of n.
 * https://oeis.org/A281118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum over all weakly increasing factorizations of n
 *            with at least two parts of the product of a(each part).
 *            (Recursive definition as in the OEIS description.)
 * Range:     u8 n <= 95, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"   /* ensure_cache */

/* Integer square‑root for a 64‑bit unsigned integer */
static inline idx_t int_sqrt_u64(idx_t n)
{
    idx_t high = n, low = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) return 0;
        if (mid > n / mid) {
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

/* Memoisation array for a(n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

static seq_t compute_a(idx_t n);

/* Depth‑first search over weakly increasing factorizations */
static void dfs(idx_t rem, idx_t min_f, seq_t prod, idx_t depth, seq_t *total)
{
    idx_t limit = int_sqrt_u64(rem);
    for (idx_t f = min_f; f <= limit; ++f) {
        if (rem % f == 0) {
            seq_t af = compute_a(f);
            seq_t new_prod = prod * af;
            dfs(rem / f, f, new_prod, depth + 1, total);
        }
    }

    /* Re‑consider rem as the last part (requires at least one earlier part). */
    if (rem >= min_f && depth >= 1) {
        seq_t ar = compute_a(rem);
        seq_t add = prod * ar;
        *total += add;
    }
}

/* Recursive definition of a(n) with memoisation */
static seq_t compute_a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    if (cache_cap <= n) {
        ensure_cache(n, &cache, &cache_cap);
    }

    if (cache[1] == 0) cache[1] = (seq_t)1;  /* initialise base case */

    if (cache[n] != 0) {
        return cache[n];
    }

    seq_t total = 0;
    dfs(n, 2, (seq_t)1, 0, &total);

    /* Add the trivial single‑node factorisation */
    cache[n] = total + (seq_t)1;
    return cache[n];
}

/* Exported function */
seq_t a(idx_t n)
{
    return compute_a(n);
}
