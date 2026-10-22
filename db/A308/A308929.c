/*
 * A308929  Sum of the fifth largest parts in the partitions of n into 7
 *          parts.
 * https://oeis.org/A308929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{o=1..floor(n/7)} Sum_{m=o..floor((n-o)/6)}
 *            Sum_{l=m..floor((n-m-o)/5)} Sum_{k=l..floor((n-l-m-o)/4)}
 *            Sum_{j=k..floor((n-k-l-m-o)/3)}
 *            Sum_{i=j..floor((n-j-k-l-m-o)/2)} l
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (Sum of the fifth largest part in the partitions of n into 7 parts) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 7)
        return 0;

    /* Arrays: dp[p][s] gives #partitions of s into exactly p parts,
       sum3[p][s] gives sum of the third part over those partitions. */
    const int P = 8;            /* indices 0..7, we need up to 7 */
    const idx_t N = n;
    size_t rowsize = (size_t)(N + 1);

    /* Allocate contiguous memory for dp and sum3 */
    seq_t *dp = (seq_t *)calloc(P * rowsize, sizeof(seq_t));
    seq_t *sum3 = (seq_t *)calloc(P * rowsize, sizeof(seq_t));
    if (!dp || !sum3) {
        /* Allocation failure – return 0 as safe fallback */
        free(dp);
        free(sum3);
        return 0;
    }

    /* dp[0][0] = 1, sum3[0][0] = 0 already by calloc */
    dp[0 * rowsize + 0] = 1;

    /* Main DP.  Outer loop over part value v 1..n. */
    for (idx_t v = 1; v <= N; ++v) {
        for (int p = 1; p <= 7; ++p) {
            seq_t *dp_prev = dp + (p - 1) * rowsize;
            seq_t *sum_prev = sum3 + (p - 1) * rowsize;
            seq_t *dp_cur  = dp +  p * rowsize;
            seq_t *sum_cur = sum3 +  p * rowsize;
            for (idx_t s = v; s <= N; ++s) {
                idx_t s_prev = s - v;
                seq_t addCnt = dp_prev[s_prev];
                if (addCnt) {
                    dp_cur[s] += addCnt;
                    /* Update sum of third parts:
                       - if p-1 == 2 : new third part is v
                       - else if p-1 >= 3 : third part unchanged
                       - otherwise : no contribution */
                    if (p - 1 == 2) {
                        sum_cur[s] += addCnt * (seq_t)v;
                    } else if (p - 1 >= 3) {
                        sum_cur[s] += sum_prev[s_prev];
                    }
                }
            }
        }
    }

    seq_t result = sum3[7 * rowsize + N];

    free(dp);
    free(sum3);
    return result;
}
