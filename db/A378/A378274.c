/*
 * A378274  Number of subgroups of S_n isomorphic to S_5 up to conjugacy,
 *          where S_n is the n-th symmetric group.
 * https://oeis.org/A378274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = number of multisets of subgroup indices of S5 summing to
 *            n with at least one index different from 1 or 2
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Protect against n < 1, though the offset starts at 1. */
    if (n < 1)
        return 0;

    /* Local cache for DP arrays. Re-use across calls if possible. */
    static seq_t *total = NULL;
    static seq_t *forbid = NULL;
    static size_t cap = 0;

    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        free(total);
        free(forbid);
        total  = (seq_t*)malloc(needed * sizeof(seq_t));
        forbid = (seq_t*)malloc(needed * sizeof(seq_t));
        cap = needed;
    }

    /* Initialize arrays to zero, then set base case. */
    for (size_t i = 0; i < needed; ++i) {
        total[i]  = 0;
        forbid[i] = 0;
    }
    total[0]  = 1;
    forbid[0] = 1;

    /* All conjugacy classes of subgroups of S5 (indices) */
    static const idx_t weights[] = {
        1, 2,          /* core-containing subgroups */
        5, 6, 10, 10,  /* core-free with multiplicities for index 10 */
        12, 15, 20, 24,
        30, 30,        /* two V4 and C4 subgroups */
        40,
        60, 60,        /* transposition and double transposition */
        120            /* trivial subgroup */
    };
    const size_t num_weights = sizeof(weights) / sizeof(weights[0]);

    /* DP for total combinations using all item types */
    for (size_t wi = 0; wi < num_weights; ++wi) {
        idx_t w = weights[wi];
        if (w > n)
            continue;
        for (idx_t s = w; s <= n; ++s)
            total[s] += total[s - w];
    }

    /* DP for forbidden combinations (only weights 1 and 2) */
    static const idx_t forbid_weights[] = {1, 2};
    for (size_t fi = 0; fi < 2; ++fi) {
        idx_t w = forbid_weights[fi];
        if (w > n)
            continue;
        for (idx_t s = w; s <= n; ++s)
            forbid[s] += forbid[s - w];
    }

    return total[n] - forbid[n];
}
