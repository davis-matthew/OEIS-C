/*
 * A308171  Limiting sequence of digits, read from the right, when starting
 *          with 5 we repeatedly replace each digit with its square (as in
 *          A308170).
 * https://oeis.org/A308171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9546, u16 n <= 9546, u32 n <= 9546, u64 n <= 9546,
 *            u128 n <= 9546, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of sequence A308171:  
   read from the right the digits of the infinite word produced by  
   repeatedly replacing each decimal digit with its square. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of the current word */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    /* a(1) is defined; behaviour for n < 1 is arbitrary */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* initialise with the first word ("5") if not yet done */
    if (seq_len == 0) {
        seq_len = 1;
        seq = (seq_t*)malloc(seq_len * sizeof(seq_t));
        seq[0] = (seq_t)5;
    }

    /* Expand until we have at least n digits */
    while (seq_len < (size_t)n) {
        size_t new_cap = seq_len * 2 + 10;         /* ample capacity */
        seq_t *new_seq = (seq_t*)malloc(new_cap * sizeof(seq_t));
        size_t new_len = 0;

        for (size_t i = 0; i < seq_len; ++i) {
            int d = (int)seq[i];
            int s = d * d;                      /* 0‑9 → 0‑81 */
            if (s >= 10) {
                new_seq[new_len++] = (seq_t)(s / 10);   /* tens digit   */
                new_seq[new_len++] = (seq_t)(s % 10);   /* ones digit   */
            } else {
                new_seq[new_len++] = (seq_t)s;          /* single digit */
            }
        }

        free(seq);
        seq = new_seq;
        seq_len = new_len;
    }

    /* a(1) is the rightmost digit, a(2) the next one, … */
    return seq[seq_len - (size_t)n];
}
