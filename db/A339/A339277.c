/*
 * A339277  Number of partitions of 2*n into powers of 2 where every part
 *          appears at least 2 times.
 * https://oeis.org/A339277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of 2*n into powers of two where each part
 *            appears at least twice. This implementation uses a DP that
 *            processes each power of two and uses a clever prefix‑sum
 *            technique to handle the “at least two copies” restriction in
 *            O(N) time per coin
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t limit = 2 * n;                     /* we need partitions of y = 2*n   */
    seq_t *dp   = (seq_t*)calloc((size_t)limit + 1, sizeof(seq_t));
    seq_t *newdp= (seq_t*)calloc((size_t)limit + 1, sizeof(seq_t));
    if (!dp || !newdp) {
        free(dp); free(newdp);
        return 0; /* OOM – unlikely for the intended ranges */
    }
    dp[0] = 1;                                /* empty partition */

    for (idx_t v = 1; v <= limit; v <<= 1) {   /* each power of two */
        for (idx_t r = 0; r < v; ++r) {
            seq_t sumAll = 0;                 /* prefix sum of dp over this residue class */
            for (idx_t s = r; s <= limit; s += v) {
                idx_t k = (s - r) / v;       /* position index within the residue class */
                if (k >= 2) {
                    /* sum the dp values for indices up to k-2
                       which is (sumAll – dp[s - v]) because sumAll includes dp[s - v] */
                    seq_t add = sumAll - dp[s - v];
                    newdp[s] = dp[s] + add;
                } else {
                    newdp[s] = dp[s];          /* use 0 or 1 copies – not allowed, so just inherit */
                }
                sumAll += dp[s];
            }
        }
        /* swap dp and newdp for next coin – newdp will be overwritten in the next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
        /* no need to clear newdp; it will be fully overwritten next round */
    }

    seq_t result = dp[limit];
    free(dp);
    free(newdp);
    return result;
}
