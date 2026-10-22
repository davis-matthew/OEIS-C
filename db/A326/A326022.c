/*
 * A326022  Number of minimal complete subsets of {1..n} with maximum n.
 * https://oeis.org/A326022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of minimal complete subsets of {1..n} with maximum
 *            n
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   is_complete, is_complete_without_removed, minimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_complete.h"
#include "../utils/is_complete_without_removed.h"
#include "../utils/minimal.h"

/* Maximum possible length of a minimal complete set for n <= 10000 */
/* For n = 10000, the longest set occurs when each new element is sum(prev)+1,
   leading to 2^k -1 ~ 10000 → k ≈ 14. 20 is a safe bound. */
#define MAX_LEN 20

/* Depth‑first search that enumerates all sequences of distinct positive integers
   starting from 'last', current total sum 'sum', and with maximum element allowed
   up to 'limit'.  Whenever a complete sequence is found, it is tested for
   minimality and the corresponding counter is incremented. */
static void dfs(idx_t last, idx_t sum, idx_t *seq, int len,
                idx_t limit, seq_t *counts)
{
    /* Count the current set if it is minimal. */
    if (minimal(seq, len))
        ++counts[last];

    /* Try to append a larger element x,  last < x <= min(limit, sum+1). */
    idx_t max_next = sum + 1;
    if (max_next > limit) max_next = limit;
    for (idx_t x = last + 1; x <= max_next; ++x) {
        seq[len] = x;
        dfs(x, sum + x, seq, len + 1, limit, counts);
    }
}

/* Compute counts for all n up to 'limit' in the array 'counts' (size limit+1). */
static void compute_counts(idx_t limit, seq_t *counts)
{
    memset(counts, 0, (size_t)(limit + 1) * sizeof(seq_t));

    idx_t seq[MAX_LEN];
    seq[0] = 1;                     /* the first element must be 1 */
    dfs(1, 1, seq, 1, limit, counts);
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Cache the computed sequence to avoid repeated work. */
    static idx_t cached_limit = 0;
    static seq_t *cached_counts = NULL;

    if (n == 0) return 0;           /* safety for n < 1 */

    if (cached_counts == NULL || n > cached_limit) {
        /* Allocate new array and compute all values up to 'n'. */
        idx_t new_limit = n;
        seq_t *new_counts = (seq_t *)malloc((size_t)(new_limit + 1) * sizeof(seq_t));
        if (!new_counts) exit(EXIT_FAILURE);
        compute_counts(new_limit, new_counts);

        /* Release old cache if it exists. */
        free(cached_counts);
        cached_counts = new_counts;
        cached_limit = new_limit;
    }

    return cached_counts[n];
}
