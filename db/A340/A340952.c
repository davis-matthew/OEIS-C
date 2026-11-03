/*
 * A340952  Number of ways to write n as an ordered sum of 7 nonzero
 *          triangular numbers.
 * https://oeis.org/A340952
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered sextuplets of non‑zero triangular
 *            numbers summing to n * Equivalent to the coefficient of x^n in
 *            (Σ_{k≥1} x^{k(k+1)/2})^7. * Implementation: DP for 7 multisets,
 *            O(7·n·T(n)) with T(n)≈√(2n) triangular numbers
 * Range:     u8 n <= 20, u16 n <= 106, u32 n <= 6313, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9648 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *
allocate_vec(idx_t n) { return mem_new(seq_t, (n + 1)); }

/* seq_t and idx_t are supplied by the build system.  */

seq_t a(idx_t n)
{
    /* For indices below the offset, return 0. */
    if (n < 7)
        return (seq_t)0;

    /* Compute all triangular numbers ≤ n. */
    idx_t tri[256];
    idx_t tri_cnt = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t t = (k * (k + 1)) / 2;   /* k*(k+1)/2 fits in uint64_t for k≤140 (n≤10000). */
        if (t > n)
            break;
        tri[tri_cnt++] = t;
    }

    /* DP arrays: dp_prev[i] = ways with current number of summands. */
    seq_t *dp_prev = allocate_vec(n);
    seq_t *dp_curr = allocate_vec(n);
    if (!dp_prev || !dp_curr) { /* Allocation failure – treat as zero. */
        mem_free(dp_prev);
        mem_free(dp_curr);
        return (seq_t)0;
    }

    /* Initialize for 0 summands. */
    mem_set(dp_prev, 0, (n + 1) * sizeof(seq_t));
    dp_prev[0] = (seq_t)1;

    /* Add 7 summands successively. */
    for (idx_t step = 0; step < 7; ++step) {
        mem_set(dp_curr, 0, (n + 1) * sizeof(seq_t));
        for (idx_t j = 0; j < tri_cnt; ++j) {
            idx_t t = tri[j];
            for (idx_t i = t; i <= n; ++i) {
                dp_curr[i] += dp_prev[i - t];
            }
        }
        /* Swap arrays for next step. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    seq_t result = dp_prev[n];
    mem_free(dp_prev);
    mem_free(dp_curr);
    return result;
}
