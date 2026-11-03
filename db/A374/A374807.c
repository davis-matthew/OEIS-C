/*
 * A374807  a(n) is the smallest number which can be represented as the sum of
 *          4 distinct nonzero triangular numbers in exactly n ways, or -1 if
 *          no such number exists.
 * https://oeis.org/A374807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A374807:  a(n) = smallest number which can be represented as
   the sum of 4 distinct non‑zero triangular numbers in exactly n ways,
   or -1 if no such number exists. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/*  Helper: DP computation that fills minSum[1..maxCount] with the
    minimal sum having exactly c representations, for c ≤ maxCount.  */
static void compute_counts(idx_t sum_limit,
                           idx_t maxCount,
                           seq_t *minSum /* size maxCount+1 */)
{
    /* allocate DP arrays for 0..4 elements */
    size_t sz = (size_t)sum_limit + 1;
    uint32_t *dp0 = (uint32_t*)calloc(sz, sizeof(uint32_t));
    uint32_t *dp1 = (uint32_t*)calloc(sz, sizeof(uint32_t));
    uint32_t *dp2 = (uint32_t*)calloc(sz, sizeof(uint32_t));
    uint32_t *dp3 = (uint32_t*)calloc(sz, sizeof(uint32_t));
    uint32_t *dp4 = (uint32_t*)calloc(sz, sizeof(uint32_t));
    if (!dp0 || !dp1 || !dp2 || !dp3 || !dp4) return; /* allocation failure */

    dp0[0] = 1;                                 /* empty sum */

    /* generate triangular numbers up to sum_limit */
    idx_t *tri = NULL;
    size_t tri_cap = 0;
    size_t tri_cnt = 0;
    for (idx_t k = 1;; ++k) {
        seq_t t = (seq_t)k * (k + 1) / 2;
        if (t > (seq_t)sum_limit) break;
        if (tri_cnt == tri_cap) {
            size_t new_cap = tri_cap ? tri_cap * 2 : 64;
            idx_t *tmp = (idx_t*)realloc(tri, new_cap * sizeof(idx_t));
            if (!tmp) { free(tri); break; }
            tri = tmp;
            tri_cap = new_cap;
        }
        tri[tri_cnt++] = (idx_t)t;
    }

    uint32_t cap = (uint32_t)(maxCount + 1);   /* maximum count recorded */

    /* DP update: iterate over triangular numbers */
    for (size_t it = 0; it < tri_cnt; ++it) {
        idx_t t = tri[it];
        for (idx_t s = sum_limit; s >= t; --s) {
            uint32_t add = dp3[s - t];
            if (add) {
                uint64_t tmp = (uint64_t)dp4[s] + add;
                dp4[s] = (tmp >= cap) ? cap : (uint32_t)tmp;
            }
            if (s == t) break;
        }
        for (idx_t s = sum_limit; s >= t; --s) {
            uint32_t add = dp2[s - t];
            if (add) {
                uint64_t tmp = (uint64_t)dp3[s] + add;
                dp3[s] = (tmp >= cap) ? cap : (uint32_t)tmp;
            }
            if (s == t) break;
        }
        for (idx_t s = sum_limit; s >= t; --s) {
            uint32_t add = dp1[s - t];
            if (add) {
                uint64_t tmp = (uint64_t)dp2[s] + add;
                dp2[s] = (tmp >= cap) ? cap : (uint32_t)tmp;
            }
            if (s == t) break;
        }
        for (idx_t s = sum_limit; s >= t; --s) {
            /* dp1 gets dp0 which never changes */
            if (dp0[s - t]) {
                uint64_t tmp = (uint64_t)dp1[s] + dp0[s - t];
                dp1[s] = (tmp >= cap) ? cap : (uint32_t)tmp;
            }
            if (s == t) break;
        }
    }

    /* collect minimal sums for each count */
    for (idx_t s = 1; (size_t)s <= sum_limit; ++s) {
        uint32_t c = dp4[s];
        if (c && c <= maxCount && minSum[(size_t)c] == (seq_t)0) {
            minSum[(size_t)c] = (seq_t)s;
        }
    }

    /* cleanup */
    free(dp0);
    free(dp1);
    free(dp2);
    free(dp3);
    free(dp4);
    free(tri);
}

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

/* maximum sum that the routine retries up to before giving up (can be tuned) */
#define MAX_SUM_LIMIT 5000000

/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*  Global state for memoization */
static idx_t g_maxRequest = 0;
static seq_t *g_minSum = NULL;   /* array of size g_maxRequest+1, 1-based */

/*  Ensure that minimal sums up to n are known */
static void ensure_computed(idx_t n)
{
    if (n <= g_maxRequest) return;                     /* already ready */

    /* we will grow the maximum request to at least n */
    idx_t target = n;
    seq_t *newMinSum = NULL;

    /* estimate initial sum limit */
    idx_t sum_limit = target * 500;                     /* heuristic */
    if (sum_limit < 1000) sum_limit = 1000;
    if (sum_limit > MAX_SUM_LIMIT) sum_limit = MAX_SUM_LIMIT;

    /* repeat until all counts are found or sum_limit reached bound */
    while (1) {
        /* allocate temporary result array */
        newMinSum = mem_new(seq_t, (size_t)target + 1);
        if (!newMinSum) return;                       /* allocation failure */

        compute_counts(sum_limit, target, newMinSum);

        /* check if all counts have a value */
        int all_found = 1;
        for (size_t i = 1; i <= target; ++i) {
            if (newMinSum[i] == (seq_t)0) {
                all_found = 0;
                break;
            }
        }

        if (all_found) {
            /* accept result */
            mem_free(g_minSum);
            g_minSum = newMinSum;
            g_maxRequest = target;
            return;
        }

        /* not all found: try a larger sum limit */
        mem_free(newMinSum);
        newMinSum = NULL;

        /* increase sum_limit exponentially but not beyond bound */
        sum_limit *= 2;
        if (sum_limit > MAX_SUM_LIMIT) {
            sum_limit = MAX_SUM_LIMIT;
        }
        if (sum_limit == MAX_SUM_LIMIT) {
            /* cannot increase further; accept partial data */
            mem_free(g_minSum);
            g_minSum = newMinSum;
            g_maxRequest = target;
            return;
        }
    }
}

/*----------------------------------------------------------------------*/
/*  Public function required by the interface */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)-1;                         /* undefined for n<1 */

    ensure_computed(n);
    seq_t res = g_minSum ? g_minSum[(size_t)n] : (seq_t)0;
    return (res == (seq_t)0) ? (seq_t)-1 : res;
}
