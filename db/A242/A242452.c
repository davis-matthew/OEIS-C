/*
 * A242452  Number of length n words on {1,2,3} with no more than one
 *          consecutive 1 and no more than two consecutive 2's and no more
 *          than three consecutive 3's.
 * https://oeis.org/A242452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) can be obtained by a 6‑state DP a(0)=1, a(1)=3, a(n)=sum of
 *            all DP states for length n. States: 0=(1,1), 1=(2,1), 2=(2,2),
 *            3=(3,1), 4=(3,2), 5=(3,3)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if !defined(__cplusplus)
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
#else
# include <cstddef>
# include <cstdlib>
# include <cstring>
#include "../utils/seq_free.h"
#endif

static seq_t* seq_alloc(size_t cap) {
    return (seq_t*)malloc(cap * sizeof(seq_t));
}

static seq_t* seq_realloc(seq_t* p, size_t old_cap, size_t new_cap) {
    if (new_cap <= old_cap) return p;
    return (seq_t*)realloc(p, new_cap * sizeof(seq_t));
}

seq_t a(idx_t n) {
    /* cache of total counts */
    static seq_t* total = NULL;
    static size_t tot_cap = 0;
    static idx_t cur_len = 0;

    /* DP arrays: 6 states */
    static seq_t dp[6] = {0}, next_dp[6] = {0};

    /* initialise cache if first call */
    if (!total) {
        tot_cap = 1;
        total = seq_alloc(tot_cap);
        total[0] = 1;                 /* a(0) */
    }

    /* initialise DP for length 1 if starting from scratch */
    if (cur_len == 0) {
        dp[0] = 1;   /* (1,1) */
        dp[1] = 1;   /* (2,1) */
        dp[3] = 1;   /* (3,1) */
        /* store a(1) */
        seq_t sum = dp[0] + dp[1] + dp[3];
        if (tot_cap < 2) {
            size_t new_cap = 2;
            seq_t* tmp = seq_realloc(total, tot_cap, new_cap);
            total = tmp;
            tot_cap = new_cap;
        }
        total[1] = sum;
        cur_len = 1;
    }

    /* grow cache up to requested n */
    for (idx_t len = cur_len; len < n; ++len) {
        memset(next_dp, 0, sizeof(next_dp));

        /* transition from each state */
        for (int i = 0; i < 6; ++i) {
            seq_t ways = dp[i];
            if (!ways) continue;

            int type, run;
            switch (i) {
                case 0: type = 1; run = 1; break;
                case 1: type = 2; run = 1; break;
                case 2: type = 2; run = 2; break;
                case 3: type = 3; run = 1; break;
                case 4: type = 3; run = 2; break;
                case 5: type = 3; run = 3; break;
                default: continue;
            }

            for (int s = 1; s <= 3; ++s) {
                if (s == type) {
                    int newrun = run + 1;
                    int limit = (type == 1 ? 1 : (type == 2 ? 2 : 3));
                    if (newrun <= limit) {
                        int idx;
                        if (type == 1)          idx = 0;
                        else if (type == 2) {
                            idx = (newrun == 1) ? 1 : 2;
                        } else { /* type 3 */
                            if (newrun == 1)      idx = 3;
                            else if (newrun == 2) idx = 4;
                            else                  idx = 5;
                        }
                        next_dp[idx] += ways;
                    }
                } else {
                    int idx = (s == 1 ? 0 : (s == 2 ? 1 : 3));
                    next_dp[idx] += ways;
                }
            }
        }

        memcpy(dp, next_dp, sizeof(dp));

        /* sum current states for total[n] */
        seq_t sum = 0;
        for (int j = 0; j < 6; ++j) sum += dp[j];

        /* ensure capacity for total */
        if (tot_cap <= (size_t)(len + 1)) {
            size_t new_cap = tot_cap > 0 ? tot_cap * 2 : 2;
            while (new_cap <= (size_t)(len + 1)) new_cap *= 2;
            seq_t* tmp = seq_realloc(total, tot_cap, new_cap);
            total = tmp;
            tot_cap = new_cap;
        }
        total[len + 1] = sum;
    }

    cur_len = n;
    return total[n];
}
