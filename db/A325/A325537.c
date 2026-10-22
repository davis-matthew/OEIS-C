/*
 * A325537  Irregular triangle whose rows are the sorted combined parts of all
 *          strict integer partitions of n.
 * https://oeis.org/A325537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A325537: the flattened triangle of sorted combined parts
 * of all strict integer partitions of n.
 *
 * The function generates rows successively until the cumulative
 * number of elements reaches the requested index.  For each row
 * r it computes the multiplicity of every part k (1 ≤ k ≤ r),
 * namely the number of strict partitions of r that contain k.
 * That multiplicity is the number of subsets of {1,…,r}\{k}
 * summing to r – k.
 *
 * The multiplicities are obtained by a standard DP over subsets
 * (each number used at most once).  With the multiplicities known,
 * the length of the row is their sum.  Once the target offset inside
 * the row is known, the value is found by scanning the parts in
 * ascending order.
 *
 * All arithmetic stays within seq_t; the DP tables are small
 * because row numbers grow very slowly (≈ 25 for index 10 000).
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1 */
    seq_t target = (seq_t)n;              /* 1‑based index in the flattened sequence */
    seq_t cumulative = 0;                 /* number of elements produced so far */

    for (idx_t r = 1; ; ++r) {            /* current row number */
        /* Allocate arrays for this row */
        size_t sz = (size_t)r + 1;        /* indices 0..r used */
        seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!dp) return 0;               /* malloc failure, safe exit */
        seq_t *mul = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!mul) { free(dp); return 0; }

        /* Compute multiplicity of each part k */
        for (idx_t k = 1; k <= r; ++k) {
            /* reset dp for this k */
            memset(dp, 0, sz * sizeof(seq_t));
            dp[0] = 1;                    /* empty subset */

            for (idx_t t = 1; t <= r; ++t) {
                if (t == k) continue;     /* exclude the part k itself */
                for (idx_t s = r; s >= t; --s)
                    dp[s] += dp[s - t];
            }
            mul[k] = dp[r - k];           /* subsets summing to r-k */
        }

        /* Compute row length and see if target lies in this row */
        seq_t row_len = 0;
        for (idx_t k = 1; k <= r; ++k) row_len += mul[k];

        if (cumulative + row_len >= target) {
            /* Target index is within this row */
            seq_t offset = target - cumulative - 1;   /* zero‑based offset in the row */
            for (idx_t k = 1; k <= r; ++k) {
                if (offset < mul[k]) {
                    free(dp);
                    free(mul);
                    return (seq_t)k;     /* part value at the position */
                }
                offset -= mul[k];
            }
            /* Should never reach here */
            free(dp);
            free(mul);
            return 0;
        }

        /* Target is further; continue to next row */
        cumulative += row_len;
        free(dp);
        free(mul);
    }
}
