/*
 * A347706  Number of factorizations of n that are not a twin (x*x) nor have
 *          an alternating permutation.
 * https://oeis.org/A347706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weakly increasing factorizations of n into
 *            integers >1 that are neither a twin (x,x) nor have an
 *            alternating permutation
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   dfsAlt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: uses only <stddef.h>, <stdint.h>, <stdbool.h>,
   <stdlib.h>; no external utilities required. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/dfsAlt.h"

/* helper forward declarations */
static bool hasAlternating(const seq_t *parts, size_t len);
static bool dfsAlt(int *cnt, int rem, int last, int pos,
                   int orient, const seq_t *vals, int uniq);

/* sentinel value for universe size   */
#define MAX_DEPTH 20

/* memoization storage */
static seq_t *memo = NULL;
static bool *memo_done = NULL;
static idx_t memo_cap = 0;

static void ensure_capacity(idx_t n) {
    if (n < memo_cap) return;
    idx_t new_cap = n + 1;
    seq_t *new_memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    bool *new_done = (bool *)realloc(memo_done, new_cap * sizeof(bool));
    if (!new_memo || !new_done) {
        /* allocation failed, abort */
        abort();
    }
    memo = new_memo;
    memo_done = new_done;
    for (idx_t i = memo_cap; i < new_cap; ++i) {
        memo_done[i] = false;
    }
    memo_cap = new_cap;
}

/* counts a(n) through recursion over factorizations */
static void dfs_factor(idx_t n, idx_t min_fac, idx_t depth,
                       idx_t *parts, size_t len, seq_t *ans) {
    if (n == 1) {
        /* we're at a factorization; evaluate */
        if (len <= 1) return;           /* length 1 or 0 not counted */
        if (len == 2 && parts[0] == parts[1]) return;  /* twin skip */

        /* copy into seq_t array for alternating check */
        seq_t vals[MAX_DEPTH];
        for (size_t i = 0; i < len; ++i) vals[i] = (seq_t)parts[i];
        if (hasAlternating(vals, len)) return;   /* alternating perm. */

        (*ans) = (*ans) + (seq_t)1;
        return;
    }

    for (idx_t f = min_fac; f <= n; ++f) {
        if (n % f == 0) {
            parts[len] = f;
            dfs_factor(n / f, f, depth + 1, parts, len + 1, ans);
        }
    }
}

/* test if a multiset of parts (weakly increasing) has an alternating permutation */
static bool hasAlternating(const seq_t *parts, size_t len) {
    if (len <= 1) return true;    /* single element is alternating */

    /* build unique values and counts */
    seq_t uniqVals[MAX_DEPTH];
    int uniqCnt[MAX_DEPTH];
    size_t uniq = 0;
    for (size_t i = 0; i < len; ++i) {
        if (i == 0 || parts[i] != parts[i - 1]) {
            uniqVals[uniq] = parts[i];
            uniqCnt[uniq] = 1;
            ++uniq;
        } else {
            ++uniqCnt[uniq - 1];
        }
    }

    /* try both orientations */
    for (int orient = 0; orient < 2; ++orient) {
        for (int start = 0; start < (int)uniq; ++start) {
            if (uniqCnt[start] == 0) continue;
            uniqCnt[start]--;
            if (dfsAlt(uniqCnt, (int)len - 1, start, 0,
                       orient, uniqVals, (int)uniq)) {
                uniqCnt[start]++;
                return true;
            }
            uniqCnt[start]++;
        }
    }
    return false;
}

/* public function as required by the build system */
seq_t a(idx_t n) {
    if (n == 0) {
        /* undefined for offset 1; return 0 safely */
        return (seq_t)0;
    }

    ensure_capacity(n);

    if (memo_done[n]) return memo[n];

    seq_t res = (seq_t)0;
    if (n > 1) {
        idx_t parts[MAX_DEPTH];
        dfs_factor(n, 2, 0, parts, 0, &res);
    }

    memo_done[n] = true;
    memo[n] = res;
    return res;
}
