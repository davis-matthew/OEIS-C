/*
 * A285568  1-limiting word of the morphism 0->11, 1-> 0110.
 * https://oeis.org/A285568
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

/* 1-limiting word of the morphism 0→11, 1→0110 (starting from 0).
   Computes a(n) as the nth symbol (1‑based) of the infinite word obtained
   from the 1‑limiting word. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *w   = NULL;   /* current word */
    static idx_t  len = 0;      /* length of w  */
    static idx_t  iter = 0;     /* current iteration */

    /* initialise with the starting word "0" */
    if (w == NULL) {
        w   = (seq_t*)malloc(sizeof(seq_t));
        if (!w) return 0;
        w[0] = 0;
        len  = 1;
        iter = 0;
    }

    /* expand until we reach an odd iteration with length ≥ n */
    while (len < n || (iter % 2 == 0)) {
        idx_t i, pos;
        /* compute length of next word */
        idx_t new_len = 0;
        for (i = 0; i < len; ++i) {
            seq_t c = w[i];
            new_len += (c == 0) ? 2 : 4;
        }

        /* allocate new word */
        seq_t *new_w = (seq_t*)malloc((size_t)new_len * sizeof(seq_t));
        if (!new_w) return 0;

        /* expand according to the morphism */
        pos = 0;
        for (i = 0; i < len; ++i) {
            seq_t c = w[i];
            if (c == 0) {
                new_w[pos++] = 1;
                new_w[pos++] = 1;
            } else { /* c == 1 */
                new_w[pos++] = 0;
                new_w[pos++] = 1;
                new_w[pos++] = 1;
                new_w[pos++] = 0;
            }
        }

        free(w);
        w   = new_w;
        len = new_len;
        ++iter;
    }

    return w[n - 1];
}
