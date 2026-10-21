/*
 * A294647  Differential variant of the Kolakoski sequence, with initial terms
 *          1, 1.
 * https://oeis.org/A294647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9457, u16 n <= 9457, u32 n <= 9457, u64 n <= 9457,
 *            u128 n <= 9457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Differential Kolakoski sequence (OEIS A294647)
 *
 *  The sequence a(1)=1, a(2)=1 and each subsequent a(n) is the length
 *  of a run in the derived difference sequence
 *  D(k)=|a(k+1)-a(k)|.  In other words
 *
 *    a(n) = length of the n‑th run of D,
 *           where the runs of D alternate between the values 0 and 1.
 *
 *  The following routine builds a(n) iteratively.  It keeps the current
 *  D value (curDiff) and the number of times that value still has to
 *  appear (run_remaining).  The length of each run is taken from the
 *  already computed terms of a.  The algorithm works in O(n) time and
 *  requires O(n) storage for the a(n) values that are needed for
 *  subsequent run lengths.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n == (idx_t)0) {
        return (seq_t)0;   /* offset 1, so a(0) is undefined; return 0 */
    }
    if (n == (idx_t)1) {
        return (seq_t)1;
    }
    if (n == (idx_t)2) {
        return (seq_t)1;
    }

    /* Allocate storage for the first n terms of a */
    seq_t *a_vals = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!a_vals) {
        /* In the unlikely event of allocation failure, return 0 */
        return (seq_t)0;
    }

    a_vals[0] = (seq_t)1;   /* a(1) */
    a_vals[1] = (seq_t)1;   /* a(2) */

    /* State variables for generating the sequence */
    idx_t run_index = 1;          /* points to the current run length in a_vals  */
    seq_t curDiff   = (seq_t)1;   /* current D value (1 after the first run of 0s)   */
    seq_t run_remaining = a_vals[run_index]; /* how many times curDiff still appears   */
    seq_t prev_val   = a_vals[1];   /* a(2) */
    idx_t i = 2;                  /* next index to fill (a(3)) */

    while (i < n) {
        /* Determine the next a value from the current D difference */
        seq_t next_val = (curDiff == (seq_t)0) ? prev_val
                                              : (seq_t)3 - prev_val; /* because values are 1 or 2 */

        a_vals[i] = next_val;
        i++;

        /* Consume one occurrence of the current D value */
        run_remaining--;
        if (run_remaining == (seq_t)0) {
            /* Move to the next run: flip the D value and use the
               just-generated term as its run length.          */
            run_index++;                       /* a[run_index] is now defined   */
            curDiff = (seq_t)1 - curDiff;      /* 0<->1 alternating           */
            run_remaining = a_vals[run_index]; /* length of the new run         */
        }

        prev_val = next_val;
    }

    seq_t result = a_vals[n - 1];
    free(a_vals);
    return result;
}
