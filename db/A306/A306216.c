/*
 * A306216  Successive concatenation of the current sequence with the first
 *          differences of the sequence, a(1) = a(2) = 1.
 * https://oeis.org/A306216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 4091, i16 n <= 8193, i32 n <= 8193, i64 n <= 8193,
 *            i128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306216: a(1)=a(2)=1; build by iteratively concatenating the
   current sequence with its first differences. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly. */
    if (n <= 2)
        return 1;

    /* Static storage for the growing sequence, shared between calls. */
    static seq_t *seq = NULL;
    static idx_t sz = 0;     /* current length of the sequence */
    static size_t cap = 0;   /* allocated capacity */

    /* Initialise the sequence on the first call. */
    if (seq == NULL) {
        cap = 8;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[0] = 1;
        seq[1] = 1;
        sz = 2;
    }

    /* Grow the sequence until it contains at least n terms. */
    while (sz < n) {
        idx_t L = sz;
        idx_t diff_len = L - 1;            /* number of differences to append */
        size_t needed = sz + diff_len;     /* new length after appending */

        /* Expand the allocated buffer if necessary. */
        if (needed > cap) {
            size_t new_cap = (cap > needed) ? cap * 2 : needed;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* Compute first differences and append them. */
        for (idx_t i = 1; i < L; ++i) {
            seq[sz + i - 1] = seq[i] - seq[i - 1];
        }

        sz = needed;   /* update the sequence length */
    }

    return seq[n - 1];
}
