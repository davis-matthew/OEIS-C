/*
 * A288381  Fixed point of the mapping 00->0001, 1->11, starting with 00.
 * https://oeis.org/A288381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol of the fixed point of the substitution
 *            rule that starts with the word “00” and then repeatedly scans
 *            the word from left to right, applying * if a 0 is followed by
 *            another 0, replace the pair “00” by “0001”; * if a 1 is
 *            encountered, replace it by “11”; * otherwise (an isolat...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;          /* current word */
static idx_t  seq_len = 0;         /* its length */

/* enlarge the current word until it contains at least n symbols */
static void ensure(idx_t n)
{
    if (seq_len >= n) return;

    /* initialise with the seed “00” if still empty */
    if (seq_len == 0) {
        seq      = (seq_t*)malloc(2 * sizeof(seq_t));
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)0;
        seq_len = 2;
    }

    /* repeat substitutions until we are long enough */
    while (seq_len < n) {
        /* first pass: compute the length of the substituted word */
        idx_t i, new_len = 0;
        for (i = 0; i < seq_len; ++i) {
            if (seq[i] == (seq_t)1) {
                new_len += 2;          /* 1 -> 11 */
            } else {                   /* seq[i] == 0 */
                if (i + 1 < seq_len && seq[i + 1] == (seq_t)0) {
                    new_len += 4;      /* 00 -> 0001 */
                    ++i;               /* skip the paired zero */
                } else {
                    new_len += 1;      /* isolated 0 stays 0 */
                }
            }
        }

        /* second pass: actually build the new word */
        seq_t *new_seq = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_seq) abort();          /* allocation failed */

        idx_t pos = 0;
        for (i = 0; i < seq_len; ++i) {
            if (seq[i] == (seq_t)1) {
                new_seq[pos++] = (seq_t)1;
                new_seq[pos++] = (seq_t)1;
            } else {                   /* seq[i] == 0 */
                if (i + 1 < seq_len && seq[i + 1] == (seq_t)0) {
                    new_seq[pos++] = (seq_t)0;
                    new_seq[pos++] = (seq_t)0;
                    new_seq[pos++] = (seq_t)0;
                    new_seq[pos++] = (seq_t)1;
                    ++i;               /* skip the paired zero */
                } else {
                    new_seq[pos++] = (seq_t)0;
                }
            }
        }

        /* replace the old word */
        free(seq);
        seq     = new_seq;
        seq_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but guard */
    ensure(n);
    return seq[n-1];
}
