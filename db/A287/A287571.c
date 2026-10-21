/*
 * A287571  Start with 0 and repeatedly substitute 0->0312, 1->3120, 2->1203,
 *          3->2031.
 * https://oeis.org/A287571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9398, u16 n <= 9398, u32 n <= 9398, u64 n <= 9398,
 *            u128 n <= 9398, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes A287571 by iteratively applying the
   morphism 0→0312, 1→3120, 2→1203, 3→2031 to a word that starts as
   "0".  The array seq stores the generated digits (0…3) as seq_t values
   so that any seq_t type (including arbitrary‑precision integers) is
   handled correctly.
   The function a(n) guarantees that a(n) is the n‑th digit of the
   infinite word, for every n ≥ 1.  The implementation uses only
   integer arithmetic and standard library functions. */

#include <stdlib.h>

/* static cache of generated digits */
static seq_t *seq = NULL;  /* array of digits 0-3, stored as seq_t */
static idx_t maxlen = 0;   /* number of digits currently stored */

/* Append one application of the morphism to the existing word.
   After the function call, seq holds a word of length maxlen*4. */
static void expand(void)
{
    idx_t oldlen = maxlen;
    idx_t newlen = oldlen * 4;
    seq_t *newseq = (seq_t *)malloc(sizeof(seq_t) * newlen);
    if (!newseq) abort();  /* out of memory */

    idx_t j = 0;
    for (idx_t i = 0; i < oldlen; ++i) {
        seq_t d = seq[i];
        /* Replace the single digit d by a block of four digits. */
        if (d == 0) {
            newseq[j++] = 0; newseq[j++] = 3; newseq[j++] = 1; newseq[j++] = 2;
        } else if (d == 1) {
            newseq[j++] = 3; newseq[j++] = 1; newseq[j++] = 2; newseq[j++] = 0;
        } else if (d == 2) {
            newseq[j++] = 1; newseq[j++] = 2; newseq[j++] = 0; newseq[j++] = 3;
        } else /* d == 3 */ {
            newseq[j++] = 2; newseq[j++] = 0; newseq[j++] = 3; newseq[j++] = 1;
        }
    }
    free(seq);
    seq = newseq;
    maxlen = newlen;
}

/* Return the n‑th term of A287571 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* for safety – OEIS offset starts at 1 */

    if (!seq) {
        /* Initialize with the starting word "0". */
        seq = (seq_t *)malloc(sizeof(seq_t));
        if (!seq) abort();
        seq[0] = 0;
        maxlen = 1;
    }

    /* Grow the word until it contains at least n digits. */
    while (maxlen < n) {
        expand();
    }

    return seq[n - 1];
}
