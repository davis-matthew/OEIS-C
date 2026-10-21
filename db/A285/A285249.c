/*
 * A285249  0-limiting word of the morphism 0->10, 1-> 0101.
 * https://oeis.org/A285249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285249: nth bit of the even‑iteration limiting word of
   the morphism 0 → 10, 1 → 0101. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of the word generated so far.
       seq_len is the current length; iter counts
       the number of morphism applications performed
       (0 for the initial word, 1 for the first expansion, etc.). */
    static seq_t *seq   = NULL;
    static idx_t  seq_len = 0;
    static idx_t  iter    = 0;   /* 0 for even, 1 for odd */

    /* Lazy initialisation of the first word "0". */
    if (seq == NULL) {
        seq = (seq_t*)malloc(sizeof(seq_t));
        seq[0] = (seq_t)0;
        seq_len = 1;
    }

    /* Expand until an even iteration has produced enough bits. */
    while (1) {
        if (seq_len >= n && ((iter & 1U) == 0))
            return seq[(size_t)(n - 1)];

        /* Count the number of 1s, because a 1 expands to 4 bits
           versus 2 bits for a 0. */
        idx_t ones = 0;
        for (idx_t i = 0; i < seq_len; ++i)
            if (seq[i] == (seq_t)1)
                ++ones;

        /* Compute the length of the expanded word. */
        idx_t new_len = seq_len * 2 + ones * 2;

        seq_t *new_seq = (seq_t*)malloc((size_t)new_len * sizeof(seq_t));
        idx_t pos = 0;

        /* Apply the morphism to build the new word. */
        for (idx_t i = 0; i < seq_len; ++i) {
            if (seq[i] == (seq_t)0) {          /* 0 → 1 0 */
                new_seq[pos++] = (seq_t)1;
                new_seq[pos++] = (seq_t)0;
            } else {                           /* 1 → 0 1 0 1 */
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)1;
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)1;
            }
        }

        free(seq);
        seq      = new_seq;
        seq_len  = new_len;
        ++iter;
    }
}
