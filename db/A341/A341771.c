/*
 * A341771  a(1) = 2; for n >= 1, a(n+1) is the largest integer k such that
 *          for some integer d > 0, a(n-j*d) = a(n) for all j < k.
 * https://oeis.org/A341771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS sequence A341771.
   Definition: a(1) = 2.
   For n ≥ 1, a(n+1) is the largest integer k such that
   ∃ d > 0 with a(n - j*d) = a(n) for all j < k,
   where the indices used in the progression are ≥ 1.
   The implementation memoises all computed terms so that calls can
   come in any order.  The algorithm runs in about O(n²) time,
   which is sufficient for n ≤ 10 000 (≈10⁸ simple integer comparisons).
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cache[1..N] == a(1)..a(N)  */
static idx_t cache_size = 0;     /* largest index computed so far */

/* Ensure that values up to n are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Allocate or enlarge cache to hold indices 0..n (0 unused). */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort as the sequence cannot be calculated. */
        abort();
    }
    cache = new_cache;

    /* Compute missing terms. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)2;          /* a(1) = 2 */
            continue;
        }

        idx_t n_prev = i - 1;                /* n in the definition */
        seq_t target   = cache[n_prev];      /* a(n) */
        seq_t best_k   = 1;                  /* at least k = 1 */

        /* Try every possible common difference d. */
        for (idx_t d = 1; d <= n_prev; ++d) {
            idx_t pos = n_prev;              /* start at a(n) */
            size_t len = 0;

            for (;;) {
                if (cache[pos] != target)
                    break;
                ++len;
                if (pos < d)                 /* would underflow next iteration */
                    break;
                pos -= d;
            }

            if (len > best_k)
                best_k = (seq_t)len;
        }

        cache[i] = best_k;
    }

    cache_size = n;
}

/* Public function to obtain a(n).  External linkage required. */
seq_t a(idx_t n)
{
    if (n < 1)        /* Undefined for n < 1; return 0 as a safe fallback. */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n];
}
