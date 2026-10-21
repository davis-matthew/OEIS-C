/*
 * A288257  Fixed point of the mapping 00->0010, 1->100, starting with 00.
 * https://oeis.org/A288257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth bit (1‑indexed) of the fixed point of * the
 *            substitution 00 → 0010, 1 → 100, starting from the seed 00
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;      /* stored bits, 0 or 1 */
static size_t seq_len = 0;     /* number of bits currently stored */

/* Expand the sequence until it contains at least need bits. */
static void seq_ensure(size_t need)
{
    if (seq_len >= need) return;        /* already long enough */

    /* initialise the seed 00 on first expansion */
    if (seq == NULL) {
        seq_len = 2;
        seq = (seq_t *)malloc(2 * sizeof(seq_t));
        seq[0] = 0;
        seq[1] = 0;
        if (seq_len >= need) return;
    }

    /* repeat the substitution until we have enough bits */
    while (seq_len < need) {
        size_t old_len = seq_len;
        /* worst case each input bit turns into 3 output bits */
        size_t new_cap = old_len * 3 + 4; /* extra safety */
        seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
        size_t new_len = 0;
        size_t i = 0;

        while (i < old_len) {
            if ((seq[i] == 0) && (i + 1 < old_len) && (seq[i + 1] == 0)) {
                /* 00 → 0010 */
                new_seq[new_len++] = 0;
                new_seq[new_len++] = 0;
                new_seq[new_len++] = 1;
                new_seq[new_len++] = 0;
                i += 2;
            } else if (seq[i] == 1) {
                /* 1 → 100 */
                new_seq[new_len++] = 1;
                new_seq[new_len++] = 0;
                new_seq[new_len++] = 0;
                i += 1;
            } else { /* seq[i] == 0 and not part of 00 */
                new_seq[new_len++] = 0;
                i += 1;
            }
        }

        free(seq);
        seq = new_seq;
        seq_len = new_len;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based; ignore n < 1 as the behaviour is unspecified. */
    seq_ensure((size_t)n);
    return seq[(size_t)n - 1];
}
