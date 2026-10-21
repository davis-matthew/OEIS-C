/*
 * A262689  a(n) = largest number k <= A000196(n) for which A002828(n-(k^2)) =
 *          A002828(n)-1.
 * https://oeis.org/A262689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k ≤ ⌊√n⌋ with A002828(n - k^2) = A002828(n)-1
 * Range:     u8 n <= 9557, u16 n <= 9557, u32 n <= 9557, u64 n <= 9557,
 *            u128 n <= 9557, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming of the Lagrange representation count (A002828). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility: integer square root for idx_t (works for n ≤ 10^4). */
static idx_t _floor_isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Allocate and reallocate utilities for seq_t. */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache for minimal‑square counts (A002828) and sequence values. */
static seq_t *min_sqs = NULL;
static seq_t *a_vals = NULL;
static size_t cache_cap = 0;

/* Ensure cache contains results up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;                    /* already computed */

    idx_t    new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= n)
        new_cap *= 2;

    /* Reallocate or allocate */
    if (min_sqs == NULL) {
        min_sqs = seq_alloc(new_cap);
        a_vals  = seq_alloc(new_cap);
    } else {
        min_sqs = seq_realloc(min_sqs, cache_cap, new_cap);
        a_vals  = seq_realloc(a_vals,  cache_cap, new_cap);
    }
    if (!min_sqs || !a_vals) {
        abort();   /* out of memory */
    }

    /* Compute new entries */
    idx_t start = cache_cap;
    if (start == 0) {
        min_sqs[0] = 0;
        a_vals[0]  = 0;
        start = 1;
    }

    for (idx_t i = start; i < new_cap; ++i) {
        /* Compute minimal number of squares for i (A002828) */
        idx_t r = _floor_isqrt_u64(i);
        if (r * r == i) {
            min_sqs[i] = (seq_t)1;
        } else {
            bool two = false;
            for (idx_t j = 1; j * j <= i; ++j) {
                idx_t rest = i - j * j;
                idx_t t = _floor_isqrt_u64(rest);
                if (t * t == rest) {
                    two = true;
                    break;
                }
            }
            if (two) {
                min_sqs[i] = (seq_t)2;
            } else {
                /* Check Legendre's 4-square theorem */
                idx_t m = i;
                while (m % 4 == 0)
                    m /= 4;
                if (m % 8 == 7)
                    min_sqs[i] = (seq_t)4;
                else
                    min_sqs[i] = (seq_t)3;
            }
        }

        /* Compute a(i) – largest k with min_sqs[i-k^2] = min_sqs[i]-1 */
        seq_t best = 0;
        seq_t cur_min = min_sqs[i];
        idx_t lim = _floor_isqrt_u64(i);
        for (idx_t k = lim; k > 0; --k) {
            idx_t j = i - k * k;
            if (min_sqs[j] == cur_min - 1) {
                best = k;
                break; /* largest k found */
            }
        }
        a_vals[i] = best;
    }

    cache_cap = new_cap;
}

/* External linkage as requested. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_vals[(size_t)n];
}
