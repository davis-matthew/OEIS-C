/*
 * A275855  Platinum mean sequence: fixed point of the morphism 0 -> 0001, 1
 *          -> 001.
 * https://oeis.org/A275855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(2-sqrt(3))) - floor((n-1)*(2-sqrt(3)))
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the fixed point of the morphism 0→0001, 1→001. */
/* This implementation generates the word iteratively until the
   desired index is present.  It works for any idx_t n ≥ 1. */

#include <stdlib.h>

/* Dynamically extend the word until it contains at least n symbols. */
static seq_t *seq = NULL;
static idx_t seq_len = 0;

seq_t a(idx_t n)
{
    /* Initialise with the seed word "0". */
    if (seq == NULL) {
        seq = (seq_t*)malloc(sizeof(seq_t));
        seq[0] = (seq_t)0;
        seq_len = 1;
    }

    /* Expand until the word is long enough. */
    while (seq_len < n) {
        /* Count zeros and ones in the current word. */
        idx_t count0 = 0, count1 = 0;
        for (idx_t i = 0; i < seq_len; ++i) {
            if (seq[i] == (seq_t)0) {
                ++count0;
            } else { /* seq[i] == 1 */
                ++count1;
            }
        }
        /* Determine the length of the new word. */
        idx_t new_len = 4 * count0 + 3 * count1;

        /* Allocate space for the new word. */
        seq_t *new_seq = (seq_t*)malloc(new_len * sizeof(seq_t));
        idx_t pos = 0;
        /* Expand each symbol according to the morphism. */
        for (idx_t i = 0; i < seq_len; ++i) {
            if (seq[i] == (seq_t)0) {
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)1;
            } else { /* seq[i] == 1 */
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)1;
            }
        }
        /* Replace the old word. */
        free(seq);
        seq = new_seq;
        seq_len = new_len;
    }

    /* Return the requested term (indices start at 1). */
    return seq[n - 1];
}
