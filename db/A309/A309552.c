/*
 * A309552  Sum of the even parts in the partitions of n into 6 parts.
 * https://oeis.org/A309552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the even parts in the partitions of n into 6 parts
 *            * (OEIS A309552). The value is obtained by dynamic programming
 *            over * all part sizes up to n, keeping separately the number of
 *            partitions and * the sum of their even parts for each possible
 *            number of parts (0…6)
 * Range:     u8 n <= 15, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* idx_t is unsigned 64‑bit; seq_t may be any unsigned integer type
       (8‑, 16‑, 32‑, 64‑ or 128‑bit, or arbitrary‑precision).                  */

    static seq_t *cnt     = NULL;   /* cnt[k][s] : #partitions of sum s with k parts   */
    static seq_t *sumeven = NULL;   /* sum[k][s] : sum of even parts over those partitions */
    static idx_t  maxn    = 0;       /* highest n that has been processed              */

    /* If we have never initialised the tables, or we need to increase the
       size to handle a larger n, rebuild the whole DP table.           */
    if (maxn < n || maxn == 0) {
        idx_t newmax = (maxn < n ? n : 0);
        size_t cols = (size_t)newmax + 1;          /* one column per possible sum 0…newmax */
        size_t size = 7 * cols;                    /* 7 rows: 0…6 parts                     */

        /* Allocate new tables; zero‑initialize with calloc.             */
        seq_t *newcnt  = (seq_t *)calloc(size, sizeof(seq_t));
        seq_t *newsume = (seq_t *)calloc(size, sizeof(seq_t));
        if (!newcnt || !newsume) {                  /* allocation failure */
            free(newcnt); free(newsume);
            return (seq_t)0;
        }

        /* Base case: one way to write 0 with 0 parts.  All other cells
           start at 0 because calloc did that for us.                 */
        newcnt[0] = 1;  /* cnt[0][0] = 1                                  */

        /* DP: iterate over part sizes in increasing order.             */
        for (idx_t m = 1; m <= newmax; ++m) {
            for (idx_t k = 1; k <= 6; ++k) {
                size_t k_idx    = (size_t)k   * cols;
                size_t prev_idx = (size_t)(k-1) * cols;
                for (idx_t s = m; s <= newmax; ++s) {
                    seq_t add_cnt  = newcnt[prev_idx + (s - m)];
                    if (!add_cnt) continue;                /* nothing to add */

                    /* Update the count of partitions.                    */
                    newcnt[k_idx + s] += add_cnt;

                    /* Update the sum of even parts.                      */
                    seq_t add_sum     = newsume[prev_idx + (s - m)];
                    seq_t even_contrib = ((m & (idx_t)1) == 0)
                                         ? add_cnt * (seq_t)m
                                         : (seq_t)0;
                    newsume[k_idx + s] += add_sum + even_contrib;
                }
            }
        }

        /* Replace old tables with the newly built ones.                  */
        free(cnt);
        free(sumeven);
        cnt      = newcnt;
        sumeven  = newsume;
        maxn     = newmax;
    }

    /* Return the required value for exactly 6 parts and sum n.           */
    size_t cols = (size_t)maxn + 1;
    return sumeven[6 * cols + n];
}
