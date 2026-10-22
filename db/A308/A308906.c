/*
 * A308906  Sum of the smallest parts in the partitions of n into 6 squarefree
 *          parts.
 * https://oeis.org/A308906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the smallest parts in the partitions of n into 6
 *            squarefree parts
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    if (n < 6) return (seq_t)0;            /* by definition the first non‑zero term is a(6) */

    /* Build list of squarefree numbers up to n */
    size_t est = (size_t)(n * 0.62) + 10;
    uint64_t *sf = (uint64_t *)malloc(est * sizeof(uint64_t));
    size_t sf_cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (is_squarefree((seq_t)i)) {
            if (sf_cnt == est) {
                est <<= 1;
                sf = (uint64_t *)realloc(sf, est * sizeof(uint64_t));
            }
            sf[sf_cnt++] = (uint64_t)i;
        }
    }

    /* Allocate DP tables: count[k][s] and sum_min[k][s] for 0 ≤ k ≤ 6, 0 ≤ s ≤ n */
    size_t len = (size_t)n + 1;
    seq_t **count = (seq_t **)malloc(7 * sizeof(seq_t *));
    seq_t **sum_min = (seq_t **)malloc(7 * sizeof(seq_t *));
    for (int k = 0; k < 7; ++k) {
        count[k]   = (seq_t *)calloc(len, sizeof(seq_t));
        sum_min[k] = (seq_t *)calloc(len, sizeof(seq_t));
    }
    count[0][0] = 1;   /* one way to sum to 0 with 0 parts */

    /* DP: iterate over squarefree values. For each value we can use it any number of times. */
    for (size_t i = 0; i < sf_cnt; ++i) {
        idx_t val = (idx_t)sf[i];
        for (int k = 0; k < 6; ++k) {   /* k parts already chosen, will add one more */
            seq_t *c_k      = count[k];
            seq_t *s_k      = sum_min[k];
            seq_t *c_k1     = count[k + 1];
            seq_t *s_k1     = sum_min[k + 1];
            for (idx_t s = 0; s + val <= n; ++s) {
                seq_t ways = c_k[s];
                if (ways == 0) continue;
                idx_t ns = s + val;
                c_k1[ns] += ways;
                if (k == 0) {
                    s_k1[ns] += ((seq_t)val) * ways;   /* smallest part is val itself */
                } else {
                    s_k1[ns] += s_k[s];                 /* smallest part unchanged */
                }
            }
        }
    }

    seq_t result = sum_min[6][n];

    /* Clean up */
    free(sf);
    for (int k = 0; k < 7; ++k) {
        free(count[k]);
        free(sum_min[k]);
    }
    free(count);
    free(sum_min);

    return result;
}
