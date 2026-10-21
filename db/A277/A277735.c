/*
 * A277735  Unique fixed point of the morphism 0 -> 01, 1 -> 20, 2 -> 0.
 * https://oeis.org/A277735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9765, u16 n <= 9765, u32 n <= 9765, u64 n <= 9765,
 *            u128 n <= 9765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: fixed point of the morphism
   0 → 01 , 1 → 20 , 2 → 0  (OEIS A277735) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    /* initialise the fixed point with the single symbol 0 */
    if (seq_len == 0) {
        seq = (seq_t *)malloc(sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)0;
        seq_len = 1;
    }

    /* grow the word until it contains at least n symbols */
    while (seq_len < (size_t)n) {
        size_t new_len = 0;
        /* first pass: compute new length */
        for (size_t i = 0; i < seq_len; ++i) {
            seq_t s = seq[i];
            if (s == 0 || s == 1)
                new_len += 2;
            else if (s == 2)
                new_len += 1;
        }

        /* second pass: produce the next generation */
        seq_t *new_seq = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_seq) abort();

        size_t pos = 0;
        for (size_t i = 0; i < seq_len; ++i) {
            seq_t s = seq[i];
            if (s == 0) {
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)1;
            } else if (s == 1) {
                new_seq[pos++] = (seq_t)2;
                new_seq[pos++] = (seq_t)0;
            } else if (s == 2) {
                new_seq[pos++] = (seq_t)0;
            }
        }

        free(seq);
        seq = new_seq;
        seq_len = new_len;
    }

    return seq[(size_t)(n - 1)];
}
