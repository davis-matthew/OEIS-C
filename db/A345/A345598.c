/*
 * A345598  Numbers that are the sum of ten fourth powers in five or more
 *          ways.
 * https://oeis.org/A345598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n-th smallest positive integer that can be written as a sum
    of ten fourth powers in at least five distinct unordered multisets.
    The implementation uses dynamic programming over the powers 1⁴,2⁴,… .
    Zero is not considered a fourth power.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static state -------------------------------------------------- */
static idx_t cur_bound = 0;          /* current maximum N processed           */
static seq_t *cur_vals   = NULL;     /* array of values with ≥5 representations */
static size_t cur_vals_len = 0;      /* number of terms stored                */

/* ---------- helper: recompute all terms up to `limit` ----------------------- */
static void recompute(idx_t limit)
{
    /* 1. list all fourth powers >0 and ≤ limit */
    idx_t *pow4 = NULL;
    size_t pow4_cnt = 0, pow4_cap = 8;
    pow4 = (idx_t*)malloc(pow4_cap * sizeof(idx_t));
    for (idx_t k = 1;; ++k) {                 /* start at 1, skip 0⁴ */
        unsigned long long p = (unsigned long long)k * k * k * k;
        if (p > limit) break;
        if (pow4_cnt == pow4_cap) {
            pow4_cap <<= 1;
            pow4 = (idx_t*)realloc(pow4, pow4_cap * sizeof(idx_t));
        }
        pow4[pow4_cnt++] = (idx_t)p;
    }

    /* 2. dynamic programming over number of summands and sum value */
    size_t sz   = (size_t)(limit + 1);          /* range of sum 0..limit   */
    size_t rows = 11;                           /* 0 .. 10 summands       */
    size_t total = rows * sz;                   /* total table entries    */

    seq_t *dp   = (seq_t*)calloc(total, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(total, sizeof(seq_t));

    dp[0] = 1;    /* one way to reach sum 0 with 0 terms */

    for (size_t pi = 0; pi < pow4_cnt; ++pi) {
        idx_t p = pow4[pi];
        /* clear next table */
        for (size_t i = 0; i < total; ++i) next[i] = 0;

        for (size_t cnt = 0; cnt <= 10; ++cnt) {
            size_t base = cnt * sz;
            for (size_t sum = 0; sum <= (size_t)limit; ++sum) {
                seq_t v = dp[base + sum];
                if ((int)v == 0) continue;
                int iv = (int)v;
                for (size_t use = 0; use <= 10 - cnt; ++use) {
                    idx_t ns = sum + (idx_t)use * p;
                    if (ns > limit) break;
                    size_t add_index = (cnt + use) * sz + ns;
                    seq_t nv = next[add_index];
                    if ((int)nv < 5) {
                        int nv_int = (int)nv + iv;
                        if (nv_int > 5) nv_int = 5;
                        next[add_index] = (seq_t)nv_int;
                    }
                }
            }
        }
        /* swap dp and next buffers */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* 3. count how many sums have ≥5 representations */
    size_t count_terms = 0;
    for (size_t s = 1; s <= (size_t)limit; ++s) {
        seq_t ways = dp[10 * sz + s];
        if ((int)ways >= 5) ++count_terms;
    }

    /* 4. allocate array and store the terms */
    free(cur_vals);
    cur_vals = (seq_t*)malloc(count_terms * sizeof(seq_t));
    cur_vals_len = 0;
    for (size_t s = 1; s <= (size_t)limit; ++s) {
        seq_t ways = dp[10 * sz + s];
        if ((int)ways >= 5) {
            cur_vals[cur_vals_len++] = (seq_t)s;
        }
    }

    /* cleanup */
    free(dp);
    free(next);
    free(pow4);
    cur_bound = limit;
}

/* ---------- public function required ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for index 0 */

    /* ensure we have precomputed at least n values */
    while (cur_vals_len < (size_t)n) {
        idx_t new_limit = cur_bound ? cur_bound * 2 : 1000;
        /* increase limit until we find enough terms */
        while (1) {
            recompute(new_limit);
            if (cur_vals_len >= (size_t)n) break;
            new_limit <<= 1;
        }
    }
    return cur_vals[n - 1];
}
