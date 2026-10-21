/*
 * A250385  Number of length n+3 0..6 arrays with no four consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A250385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A250385:  Number of length n+3 0..6 arrays with no four consecutive terms
    having the maximum of any two terms equal to the minimum of the remaining
    two terms.  The sequence is computed by a linear‑time DP on the last
    three symbols.  Initial value: a(1)=1806. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Helper: quadruple-to-index */
    static const int PU = 7;                /* alphabet size */
    /* Map triple (a,b,c) -> index [0,342] */
    static const idx_t TR_MAX = 49;         /* PU*PU */
    static const idx_t TR_BASE = 7;         /* PU */
    /* Map quadruple -> good status */
    static bool good[2401];
    static bool good_inited = false;
    if (!good_inited) {
        good_inited = true;
        for (int a = 0; a < PU; ++a)
            for (int b = 0; b < PU; ++b)
                for (int c = 0; c < PU; ++c)
                    for (int d = 0; d < PU; ++d) {
                        /* Check forbidden pattern */
                        int v[4] = {a, b, c, d};
                        bool bad = false;
                        for (int i = 0; i < 4 && !bad; ++i)
                            for (int j = i + 1; j < 4 && !bad; ++j) {
                                int max_pair = v[i] > v[j] ? v[i] : v[j];
                                int rem1 = -1, rem2 = -1;
                                for (int k = 0; k < 4; ++k)
                                    if (k != i && k != j) {
                                        if (rem1 == -1) rem1 = v[k];
                                        else rem2 = v[k];
                                    }
                                int min_rem = rem1 < rem2 ? rem1 : rem2;
                                if (max_pair == min_rem)
                                    bad = true;
                            }
                        int idx = a * 343 + b * 49 + c * 7 + d;
                        good[idx] = !bad;
                    }
    }

    /* State for memoization and DP */
    static seq_t *cache = NULL;     /* Array of a(1), a(2), … */
    static idx_t cache_len = 0;     /* Number of computed terms */
    static idx_t cache_cap = 0;     /* Allocated size of cache    */

    static seq_t *dp_prev = NULL;   /* counts for current length */
    static seq_t *dp_curr = NULL;   /* temporary for next length */
    const idx_t STATE = 343;        /* 7^3 */

    if (n == 0)
        return 0;                   /* offset starts at 1 */

    /* Allocate cache if needed */
    if (cache_len < n) {
        if (!cache) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        while (cache_len < n) {
            /* Ensure cache capacity */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            if (cache_len == 0) {
                /* First initialization: length = 4 */
                dp_prev = (seq_t *)calloc(STATE, sizeof(seq_t));
                dp_curr = (seq_t *)calloc(STATE, sizeof(seq_t));
                for (int a = 0; a < PU; ++a)
                    for (int b = 0; b < PU; ++b)
                        for (int c = 0; c < PU; ++c)
                            for (int d = 0; d < PU; ++d) {
                                int qidx = a * 343 + b * 49 + c * 7 + d;
                                if (good[qidx]) {
                                    int tidx = b * 49 + c * 7 + d;
                                    dp_prev[tidx] += (seq_t)1;
                                }
                            }
                /* Total for length 4 */
                seq_t total = 0;
                for (idx_t s = 0; s < STATE; ++s) total += dp_prev[s];
                cache[cache_len++] = total;          /* a(1) */
            } else {
                /* Transition to next length */
                memset(dp_curr, 0, STATE * sizeof(seq_t));
                for (int a = 0; a < PU; ++a)
                    for (int b = 0; b < PU; ++b)
                        for (int c = 0; c < PU; ++c) {
                            int prev_idx = a * 49 + b * 7 + c;
                            seq_t val = dp_prev[prev_idx];
                            if (!val) continue;
                            for (int d = 0; d < PU; ++d) {
                                int qidx = a * 343 + b * 49 + c * 7 + d;
                                if (!good[qidx]) continue;
                                int next_idx = b * 49 + c * 7 + d;
                                dp_curr[next_idx] += val;
                            }
                        }
                /* Sum for current length */
                seq_t total = 0;
                for (idx_t s = 0; s < STATE; ++s) total += dp_curr[s];
                cache[cache_len++] = total;          /* a(cache_len) */
                /* Swap DP arrays for next iteration */
                seq_t *tmp = dp_prev;
                dp_prev = dp_curr;
                dp_curr = tmp;
            }
        }
    }
    return cache[n - 1];                      /* a(1) is cache[0] */
}
