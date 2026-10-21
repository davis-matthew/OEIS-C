/*
 * A288173  Fixed point of the mapping 00->0010, 1->001, starting with 00.
 * https://oeis.org/A288173
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

/* Fixed point of the substitution
   00 → 0010, 1 → 001
   starting from the initial word "00".
   seq_t a(idx_t n) returns the n‑th (1‑based) bit of
   the resulting binary word.  The function builds the
   word lazily and keeps a static cache so that later
   calls are fast, even if the indices are not requested
   in increasing order. */
#include <stdlib.h>

/* Ensure the static cache can hold at least n bits.
   The cache is expanded by repeatedly applying the
   substitution until its length reaches n. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* cached prefix of the fixed point */
    static size_t len  = 0;     /* current length of seq, 0 at start */

    /* initialise the cache with the seed word "00" */
    if (seq == NULL) {
        seq = (seq_t *)malloc(2 * sizeof(seq_t));
        seq[0] = 0;
        seq[1] = 0;
        len   = 2;
    }

    /* Expand the cache until it contains at least n bits */
    while (len < (size_t)n) {
        /* First pass: compute the length of the word after expansion */
        size_t new_len = 0;
        size_t i = 0;
        while (i < len) {
            if (seq[i] == 0 && i + 1 < len && seq[i + 1] == 0) {
                new_len += 4;          /* 00 → 0010 */
                i += 2;
            } else if (seq[i] == 1) {
                new_len += 3;          /* 1  → 001  */
                i += 1;
            } else { /* isolated 0 */
                new_len += 1;          /* single 0 remains 0 */
                i += 1;
            }
        }

        /* Allocate a new array of the computed size */
        seq_t *next = (seq_t *)malloc(new_len * sizeof(seq_t));
        size_t j = 0;
        i = 0;
        while (i < len) {
            if (seq[i] == 0 && i + 1 < len && seq[i + 1] == 0) {
                next[j++] = 0;
                next[j++] = 0;
                next[j++] = 1;
                next[j++] = 0;
                i += 2;
            } else if (seq[i] == 1) {
                next[j++] = 0;
                next[j++] = 0;
                next[j++] = 1;
                i += 1;
            } else { /* isolated 0 */
                next[j++] = 0;
                i += 1;
            }
        }

        /* Replace the old cache with the newly expanded one */
        free(seq);
        seq = next;
        len = new_len;
    }

    return seq[n - 1];
}
