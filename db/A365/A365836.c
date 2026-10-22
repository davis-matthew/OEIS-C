/*
 * A365836  a(1)=1; thereafter extend the sequence by appending its
 *          nonincreasing run transform, recompute the nonincreasing run
 *          transform, append it, and so on.
 * https://oeis.org/A365836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9755, u16 n <= 9755, u32 n <= 9755, u64 n <= 9755,
 *            u128 n <= 9755, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A365836
   a(1) = 1;
   For n > 1, repeatedly append the nonincreasing run transform of the
   current sequence.  The nonincreasing run transform replaces each
   maximal run of weakly decreasing terms with a single value equal to
   the length of that run.  The routine memoises the sequence so any
   call order is allowed. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *seq = NULL;   /* sequence array */
    static idx_t seq_len = 0;   /* number of elements stored   */
    static idx_t seq_cap = 0;   /* allocated capacity          */

    /* initialise the sequence with the first term if this is the first call */
    if (seq_len == 0) {
        seq_cap = 16;                                     /* start capacity   */
        seq = (seq_t *)malloc((size_t)seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;                                /* a(1) = 1         */
        seq_len = 1;
    }

    /* grow sequence until we have at least n elements */
    while (seq_len < n) {
        /* compute the nonincreasing run transform of seq[0 .. seq_len-1] */
        seq_t *tmp = (seq_t *)malloc((size_t)seq_len * sizeof(seq_t));
        idx_t tmp_len = 0;

        idx_t i = 0;
        while (i < seq_len) {
            idx_t run = 1;                                 /* at least one element in a run */
            while (i + 1 < seq_len && seq[i + 1] <= seq[i]) {
                run++;
                i++;
            }
            tmp[tmp_len++] = (seq_t)run;                   /* store run length       */
            i++;                                          /* move past last element */
        }

        /* ensure capacity to append the new run‑length block */
        if (seq_len + tmp_len > seq_cap) {
            idx_t new_cap = seq_cap * 2;
            while (new_cap < seq_len + tmp_len)
                new_cap *= 2;
            seq = (seq_t *)realloc((void *)seq,
                                    (size_t)new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* append the run‑length block to seq */
        for (idx_t j = 0; j < tmp_len; ++j)
            seq[seq_len++] = tmp[j];

        free(tmp);
    }

    /* return the requested element (1‑based index) */
    return seq[n - 1];
}
