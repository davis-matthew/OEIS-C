/*
 * A278621  Number of nX2 0..2 arrays with rows in nondecreasing lexicographic
 *          order and columns in nonincreasing lexicographic order, but with
 *          exactly one mistake.
 * https://oeis.org/A278621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 arrays over {0,1,2} with rows in
 *            nondecreasing lexicographic order and columns in nonincreasing
 *            lexicographic order, but with exactly one violation among the
 *            row comparisons (n‑1 of them) and the column comparison (1 of
 *            them)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 116, u128 n <= 210,
 *            big any n
 * Helpers:   zero_dp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/zero_dp.h"

/* Helper arrays for the 9 ordered pairs (a,b) with a,b∈{0,1,2}.  */
static const uint8_t pairA[9] = {0,0,0,1,1,1,2,2,2};
static const uint8_t pairB[9] = {0,1,2,0,1,2,0,1,2};

/* DP tables: dp_cur   – counts for the current length
   dp_next   – next-step counts (temporary).  */
static seq_t dp_cur[9][3][3];
static seq_t dp_next[9][3][3];

/* Cache for the already computed sequence values and the
   current DP length.  */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;      /* largest index computed and stored.  */
static idx_t dp_len    = 0;      /* length of arrays represented by dp_cur.  */

/* Compute a value for length 'len' from dp_cur (currently length len).
   The result is stored into seq_cache[len].  */
static void compute_and_store(idx_t len) {
    seq_t ans = 0;
    for (int p = 0; p < 9; ++p)
        for (int s = 0; s < 3; ++s)
            for (int f = 0; f < 3; ++f) {
                seq_t cnt = dp_cur[p][s][f];
                if (cnt == 0) continue;
                if ((s == 2 && f == 0) || (s != 2 && f == 1))
                    ans += cnt;
            }
    seq_cache[len] = ans;
}

/* Transition from dp_cur (length cur_len) to dp_next (length cur_len+1).  */
static void do_transition(void) {
    zero_dp(dp_next);
    for (int prev = 0; prev < 9; ++prev) {
        for (int s = 0; s < 3; ++s) {
            for (int f = 0; f < 3; ++f) {
                seq_t cnt = dp_cur[prev][s][f];
                if (cnt == 0) continue;
                for (int cur = 0; cur < 9; ++cur) {
                    int nf = f + (prev > cur);   /* row comparison violation */
                    if (nf > 2) continue;

                    int ns = s;
                    if (s == 0) {                /* still equal so far */
                        int a = pairA[cur], b = pairB[cur];
                        if (a > b) ns = 1;
                        else if (a < b) ns = 2;
                        /* else remain 0 */
                    }
                    dp_next[cur][ns][nf] += cnt;
                }
            }
        }
    }
    /* copy dp_next back to dp_cur */
    for (int p = 0; p < 9; ++p)
        for (int s = 0; s < 3; ++s)
            for (int f = 0; f < 3; ++f)
                dp_cur[p][s][f] = dp_next[p][s][f];
}

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* offset starts at 1; undefined below */

    /* Allocate or expand cache array if necessary */
    if (seq_cache == NULL || cache_size < n) {
        idx_t new_size = n + 1;        /* allocate index 0..n */
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        seq_cache = tmp;
        /* initialize new entries to 0 */
        for (idx_t i = cache_size + 1; i <= n; ++i)
            seq_cache[i] = 0;
        cache_size = new_size - 1;
    }

    /* Initialize DP tables if this is the first call */
    if (dp_len == 0) {
        zero_dp(dp_cur);
        for (int p = 0; p < 9; ++p) {
            int a0 = pairA[p], b0 = pairB[p];
            int status = (a0 > b0) ? 1 : (a0 < b0) ? 2 : 0;
            dp_cur[p][status][0] = 1;            /* length = 1, no row failures */
        }
        dp_len = 1;
        compute_and_store(1);                    /* store a(1) */
    }

    /* Extend DP to reach desired n */
    for (idx_t len = dp_len + 1; len <= n; ++len) {
        do_transition();            /* dp_cur now represents length 'len' */
        compute_and_store(len);
    }
    dp_len = n;   /* now dp_cur corresponds to length n */

    return seq_cache[n];
}
