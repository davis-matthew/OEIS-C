/*
 * A285666  Fixed point of the mapping 00->001, 1->010, starting with 00.
 * https://oeis.org/A285666
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

/* Sequence A285666: fixed point of the iterative rule
 * 00 -> 001, 1 -> 010, with isolated 0’s unchanged.
 * a(n) is the n‑th bit (0 or 1) of the limit string, 1‑indexed.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the generated sequence */
    static seq_t *seq = NULL;    /* the current string */
    static idx_t seq_len = 0;    /* current length */
    static idx_t seq_cap = 0;    /* allocated capacity */

    /* initialise with the seed "00" on first call */
    if (seq == NULL) {
        seq_cap = 2;
        seq_len = 2;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 0;
        seq[1] = 0;
    }

    /* grow the sequence until it contains position n */
    while (seq_len < n) {
        /* next string will never be longer than 3 * seq_len */
        idx_t new_cap = seq_len * 3 + 10;
        seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
        idx_t new_len = 0;
        idx_t i = 0;
        while (i < seq_len) {
            if (seq[i] == 1) {
                /* 1 -> 010 */
                new_seq[new_len++] = 0;
                new_seq[new_len++] = 1;
                new_seq[new_len++] = 0;
                i++;
            } else { /* seq[i] == 0 */
                if (i + 1 < seq_len && seq[i + 1] == 0) {
                    /* 00 -> 001 (consume both zeros) */
                    new_seq[new_len++] = 0;
                    new_seq[new_len++] = 0;
                    new_seq[new_len++] = 1;
                    i += 2;
                } else {
                    /* isolated 0 stays as 0 */
                    new_seq[new_len++] = 0;
                    i++;
                }
            }
        }
        free(seq);
        seq = new_seq;
        seq_len = new_len;
        seq_cap = new_cap;
    }

    /* return the n-th symbol (1‑indexed) */
    return (seq_t) seq[n - 1];
}
