/*
 * A271370  Total number of inversions in all partitions of n.
 * https://oeis.org/A271370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all partitions λ of n of the number of
 *            inversions * in λ (pairs i<j with λ_i > λ_j)
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 84, u64 n <= 311,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    idx_t N = n;
    size_t sz = (size_t)N + 1;

    /* allocate DP tables */
    seq_t *p_prev   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *p_cur    = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *inv_prev = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *inv_cur  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *parts_prev = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *parts_cur  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *eq_prev    = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *eq_cur     = (seq_t*)calloc(sz, sizeof(seq_t));

    /* initial state: only empty partition */
    p_prev[0] = (seq_t)1;  /* partition of 0 */

    /* iterate over allowed part size k */
    for (idx_t k = 1; k <= N; ++k) {
        /* carry over data that does not depend on the new part size */
        memcpy(p_cur,   p_prev, sz * sizeof(seq_t));
        memcpy(inv_cur, inv_prev, sz * sizeof(seq_t));
        memcpy(parts_cur, parts_prev, sz * sizeof(seq_t));

        /* multiplicity of part size k starts from zero */
        memset(eq_cur, 0, sz * sizeof(seq_t));

        /* build contributions from partitions that use part k */
        for (idx_t i = k; i <= N; ++i) {
            seq_t add_p     = p_cur[i - k];
            seq_t add_inv   = inv_cur[i - k];
            seq_t add_parts = parts_cur[i - k];
            seq_t add_eq    = eq_cur[i - k];

            p_cur[i] = p_cur[i] + add_p;
            /* each added part k creates inversions with all previous parts
               that are smaller than k. That number equals
               parts_k(n-k) - eq_k(n-k). */
            inv_cur[i] = inv_cur[i] + add_inv + add_parts - add_eq;

            parts_cur[i] = parts_cur[i] + add_parts + add_p;   /* add the new part */
            eq_cur[i]    = eq_cur[i] + add_eq + add_p;        /* new part contributes one to its own size */
        }

        /* swap current and previous tables for the next iteration */
        seq_t *tmp;
        tmp = p_prev;  p_prev = p_cur;  p_cur = tmp;
        tmp = inv_prev; inv_prev = inv_cur; inv_cur = tmp;
        tmp = parts_prev; parts_prev = parts_cur; parts_cur = tmp;
        tmp = eq_prev; eq_prev = eq_cur; eq_cur = tmp;
    }

    seq_t result = inv_prev[N];   /* total inversions for all partitions of n */

    /* free memory */
    free(p_prev); free(p_cur);
    free(inv_prev); free(inv_cur);
    free(parts_prev); free(parts_cur);
    free(eq_prev); free(eq_cur);

    return result;
}
