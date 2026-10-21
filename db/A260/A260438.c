/*
 * A260438  Row index to A255545: If n is k-th Lucky number then a(n) = k,
 *          otherwise a(n) = number of the stage where n is removed in Lucky
 *          sieve.
 * https://oeis.org/A260438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1830, u16 n <= 9156, u32 n <= 9156, u64 n <= 9156,
 *            u128 n <= 9156, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A260438 (row index to A255545).  For all n ≥ 1:
   a(n) = k  if n is the k-th lucky number,
   otherwise a(n) = the stage of the lucky sieve at which n is removed.
   The algorithm builds the lucky sieve up to the largest n requested,
   retaining the removal stage for every number and the index within
   the lucky number list for every surviving number. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static idx_t max_precomputed = 0;
    static idx_t *stage_arr = NULL;   /* stage_at[n] = 0 if never removed */
    static idx_t *luckyIdx = NULL;    /* luckyIdx[n] = 0 if not lucky */

    if (n == 0)  /* not used, just for completeness */
        return 0;

    /* Recompute sieve up to n if necessary */
    if (n > max_precomputed) {
        /* free old tables */
        free(stage_arr);
        free(luckyIdx);

        /* Allocate new tables for range [0..n] */
        idx_t N = n;
        stage_arr = (idx_t *)calloc((size_t)(N + 1), sizeof(idx_t));
        luckyIdx = (idx_t *)calloc((size_t)(N + 1), sizeof(idx_t));

        /* Stage 1: remove all even numbers */
        for (idx_t i = 2; i <= N; i += 2)
            stage_arr[i] = 1;

        /* Buffers for the currently surviving numbers (odd numbers initially) */
        idx_t bufSize = N / 2 + 2;            /* enough space for all odds */
        idx_t *buf1 = (idx_t *)malloc((size_t)bufSize * sizeof(idx_t));
        idx_t *buf2 = (idx_t *)malloc((size_t)bufSize * sizeof(idx_t));
        idx_t *surv = buf1;                  /* current survivors */
        idx_t *next = buf2;                  /* buffer for survivors of next stage */
        idx_t surv_len = 0;
        for (idx_t i = 1; i <= N; i += 2)
            surv[surv_len++] = i;            /* keep all odds */

        /* Lucky sieve stages */
        idx_t stageNum = 2;   /* removal stage for the step taken from survivors[1] */
        idx_t posIndex = 1;   /* index into surv that gives the current step */
        while (posIndex < surv_len) {
            idx_t step = surv[posIndex];
            if (step > (idx_t)surv_len)
                break;        /* no more removals possible */

            idx_t next_len = 0;
            for (idx_t idx = 0; idx < surv_len; ++idx) {
                /* positions are 1-based */
                if ((idx + 1) % step == 0) {
                    idx_t val = surv[idx];
                    if (val <= N)
                        stage_arr[val] = stageNum;
                } else {
                    next[next_len++] = surv[idx];
                }
            }

            /* Prepare for next stage */
            idx_t *tmp = surv;
            surv = next;
            next = tmp;
            surv_len = next_len;
            ++posIndex;
            ++stageNum;
        }

        /* The remaining survivors are the lucky numbers */
        for (idx_t i = 0; i < surv_len; ++i)
            luckyIdx[surv[i]] = i + 1;

        /* Clean up temporary buffers */
        free(buf1);
        free(buf2);

        max_precomputed = N;
    }

    /* Return the appropriate value */
    if (luckyIdx[n] != 0)
        return (seq_t)luckyIdx[n];
    return (seq_t)stage_arr[n];
}
