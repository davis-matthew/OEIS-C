/*
 * A284369  Fixed point of the morphism 0->1, 1->1001.
 * https://oeis.org/A284369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9366, u16 n <= 9366, u32 n <= 9366, u64 n <= 9366,
 *            u128 n <= 9366, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the morphism 0→1, 1→1001; returns the n‑th
   (1‑indexed) symbol of the infinite word. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *w = NULL;   /* word built so far */
    static idx_t len = 0;     /* current length of w */

    /* initialise with the one‑letter word "1" */
    if (w == NULL) {
        w = (seq_t *)malloc(sizeof(seq_t));
        w[0] = 1;
        len = 1;
    }

    /* grow the word until it contains the requested position n   */
    while (len < n) {
        idx_t new_len = 0;
        for (idx_t i = 0; i < len; ++i)
            new_len += (w[i] == 0) ? 1 : 4;          /* 0→1 (len = 1),
                                                        1→1001 (len = 4) */

        seq_t *neww = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (w[i] == 0) {
                neww[pos++] = 1;                     /* 0→1 */
            } else {
                neww[pos++] = 1;                     /* 1→1001 */
                neww[pos++] = 0;
                neww[pos++] = 0;
                neww[pos++] = 1;
            }
        }

        free(w);
        w     = neww;
        len   = new_len;
    }

    return w[n - 1];
}
