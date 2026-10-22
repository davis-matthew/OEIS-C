/*
 * A348526  Number of partitions of n into two or more triangular numbers.
 * https://oeis.org/A348526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of n into two or more triangular numbers.
   The DP array dp_cache stores the number of partitions of each
   integer into triangular numbers (including the empty partition for n=0).
   For a non‑triangular n, a(n) = dp_cache[n],
   otherwise a(n) = dp_cache[n] – 1 (exclude the one‑part partition).   */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *dp_cache = NULL;   /* dp_cache[i] = number of partitions of i into triangular numbers */
static idx_t  dp_cap   = 0;      /* current size of dp_cache, i.e. dp_cap = max_n + 1 */

/* Test whether n is a triangular number. */
static bool is_triangular(idx_t n)
{
    seq_t x = (seq_t)8 * n + 1;
    seq_t s = floor_sqrt(x);
    return s * s == x;
}

/* Rebuild dp_cache up to max_n (inclusive).  The old cache is freed. */
static void recompute_dp(idx_t max_n)
{
    idx_t new_cap = max_n + 1;
    /* allocate a fresh array that is safe for both C and C++ */
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
#endif
    if (!new_cache) exit(EXIT_FAILURE); /* unrecoverable */

    /* initialise */
    for (idx_t i = 0; i < new_cap; ++i) new_cache[i] = 0;
    new_cache[0] = 1; /* empty partition of 0 */

    /* DP over triangular numbers t = k(k+1)/2 */
    for (idx_t k = 1; ; ++k) {
        seq_t t = (seq_t)k * (k + 1) / 2;
        if (t > max_n) break;
        idx_t t_idx = (idx_t)t;   /* t fits into indexed type */

        for (idx_t i = t_idx; i < new_cap; ++i) {
            new_cache[i] += new_cache[i - t_idx];
        }
    }

    /* free old cache */
#ifdef __cplusplus
    delete[] dp_cache;
#else
    free(dp_cache);
#endif
    dp_cache = new_cache;
    dp_cap   = new_cap;
}

/* External linkage: compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* by definition of the sequence */

    if (n >= dp_cap)                /* need more space / recomputation */
        recompute_dp(n);

    seq_t res = dp_cache[n];
    if (is_triangular(n))
        res -= 1;                    /* exclude the 1‑part partition (n itself) */
    return res;
}
