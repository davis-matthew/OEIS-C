/*
 * A326412  Number of inversion sequences of length n where all consecutive
 *          subsequences i,j,k satisfy i >= j <= k or i <= j >= k.
 * https://oeis.org/A326412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of inversion sequences of length n in which every
 *            consecutive triple i,j,k satisfies i >= j <= k or i <= j >= k
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 1, a(2) = 2 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    idx_t cap = n + 1;            /* maximum index that can appear as a value */
    /* current DP arrays ---------------------------------------------- */
    seq_t *less_cur   = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *equal_cur  = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *greater_cur= (seq_t *)calloc(cap, sizeof(seq_t));

    /* initialise for length = 2 */
    less_cur[1]   = 1;          /* sequence (0,1) : previous < current */
    equal_cur[0]  = 1;          /* sequence (0,0) : previous == current */

    /* reusable arrays for next iteration and difference accumulators */
    seq_t *less_next   = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *equal_next  = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *greater_next= (seq_t *)calloc(cap, sizeof(seq_t));

    seq_t *diff_less   = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *diff_equal  = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *diff_greater= (seq_t *)calloc(cap, sizeof(seq_t));

    for (idx_t len = 3; len <= n; ++len) {
        /* zero difference arrays for current length */
        memset(diff_less,   0, sizeof(seq_t)*cap);
        memset(diff_equal,  0, sizeof(seq_t)*cap);
        memset(diff_greater,0, sizeof(seq_t)*cap);

        idx_t limit = len - 1;      /* current arrays have indices 0..len-2 */

        for (idx_t j = 0; j < limit; ++j) {
            seq_t L = less_cur[j];
            seq_t E = equal_cur[j];
            seq_t G = greater_cur[j];

            /* transitions from 'less' state (previous < current) */
            if (L) {
                /* next value k <= j (range 0..j-1) becomes a 'greater' state */
                if (j > 0) {
                    diff_greater[0]   += L;
                    diff_greater[j]   -= L;
                }
                /* k == j becomes an 'equal' state */
                diff_equal[j]     += L;
                diff_equal[j+1]   -= L;
            }

            /* transitions from 'greater' state (previous > current) */
            if (G) {
                /* next value k >= j (range j+1..len-1) becomes a 'less' state */
                if (j + 1 < len) {
                    diff_less[j+1]   += G;
                    diff_less[len]   -= G;
                }
                /* k == j becomes an 'equal' state */
                diff_equal[j]     += G;
                diff_equal[j+1]   -= G;
            }

            /* transitions from 'equal' state (previous == current) */
            if (E) {
                /* next value k <= j-1 becomes a 'greater' state */
                if (j > 0) {
                    diff_greater[0]   += E;
                    diff_greater[j]   -= E;
                }
                /* k == j becomes an 'equal' state */
                diff_equal[j]     += E;
                diff_equal[j+1]   -= E;
                /* next value k >= j+1 becomes a 'less' state */
                if (j + 1 < len) {
                    diff_less[j+1]   += E;
                    diff_less[len]   -= E;
                }
            }
        }

        /* prefix sums to obtain next arrays (size = len) */
        seq_t cum = 0;
        for (idx_t i = 0; i < len; ++i) {
            cum += diff_less[i];
            less_next[i] = cum;
        }
        cum = 0;
        for (idx_t i = 0; i < len; ++i) {
            cum += diff_equal[i];
            equal_next[i] = cum;
        }
        cum = 0;
        for (idx_t i = 0; i < len; ++i) {
            cum += diff_greater[i];
            greater_next[i] = cum;
        }

        /* swap current and next arrays for next iteration */
        seq_t *tmp = less_cur;   less_cur = less_next;   less_next = tmp;
        tmp = equal_cur;  equal_cur = equal_next;  equal_next = tmp;
        tmp = greater_cur;greater_cur = greater_next;greater_next = tmp;
    }

    /* sum all entries in the final arrays to obtain a(n) */
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        result += less_cur[i];
        result += equal_cur[i];
        result += greater_cur[i];
    }

    /* free allocated memory */
    free(less_cur);
    free(equal_cur);
    free(greater_cur);
    free(less_next);
    free(equal_next);
    free(greater_next);
    free(diff_less);
    free(diff_equal);
    free(diff_greater);

    return result;
}
