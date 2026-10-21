/*
 * A287556  Start with 0 and repeatedly substitute 0->0132, 1->1320, 2->3201,
 *          3->2013.
 * https://oeis.org/A287556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9348, u16 n <= 9348, u32 n <= 9348, u64 n <= 9348,
 *            u128 n <= 9348, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the morphism 0→0132, 1→1320, 2→3201, 3→2013
   (sequence A287556). */
#include <stdlib.h>

/* Returns the n‑th term of the sequence (1‑indexed).  n ≥ 1. */
seq_t a(idx_t n)
{
    /* static cache holding the fixed point sequence */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;          /* current length of seq   */

    /* initialise the sequence with the starting symbol 0 */
    if (seq_len == 0) {
        seq_len = 1;
        seq     = (seq_t *)malloc(seq_len * sizeof(seq_t));
        seq[0]  = (seq_t)0;
    }

    /* grow the sequence until it contains at least n terms */
    if ((size_t)n > seq_len) {
        while ((size_t)n > seq_len) {
            size_t new_len = seq_len * 4;
            seq_t *dst = (seq_t *)malloc(new_len * sizeof(seq_t));

            for (size_t i = 0; i < seq_len; ++i) {
                seq_t v = seq[i];
                if (v == (seq_t)0) {               /* 0 → 0 1 3 2 */
                    dst[i * 4 + 0] = (seq_t)0;
                    dst[i * 4 + 1] = (seq_t)1;
                    dst[i * 4 + 2] = (seq_t)3;
                    dst[i * 4 + 3] = (seq_t)2;
                } else if (v == (seq_t)1) {        /* 1 → 1 3 2 0 */
                    dst[i * 4 + 0] = (seq_t)1;
                    dst[i * 4 + 1] = (seq_t)3;
                    dst[i * 4 + 2] = (seq_t)2;
                    dst[i * 4 + 3] = (seq_t)0;
                } else if (v == (seq_t)2) {        /* 2 → 3 2 0 1 */
                    dst[i * 4 + 0] = (seq_t)3;
                    dst[i * 4 + 1] = (seq_t)2;
                    dst[i * 4 + 2] = (seq_t)0;
                    dst[i * 4 + 3] = (seq_t)1;
                } else {                           /* 3 → 2 0 1 3 */
                    dst[i * 4 + 0] = (seq_t)2;
                    dst[i * 4 + 1] = (seq_t)0;
                    dst[i * 4 + 2] = (seq_t)1;
                    dst[i * 4 + 3] = (seq_t)3;
                }
            }

            free(seq);
            seq     = dst;
            seq_len = new_len;
        }
    }

    return seq[n - 1];   /* 1‑indexed access */
}
