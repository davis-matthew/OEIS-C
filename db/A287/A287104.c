/*
 * A287104  Start with 0 and repeatedly substitute 0->10, 1->12, 2->0.
 * https://oeis.org/A287104
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

/* Sequence A287104: the fixed point of the morphism
   0 → 10, 1 → 12, 2 → 0.
   a(1) is the first digit of the infinite word produced by iterating this
   substitution; the word is built from the initial symbol 1. */

#include <stdlib.h>

/* static cache of the current word */
static seq_t *w = NULL;    /* array of digits 0,1,2 */
static idx_t  w_len = 0;   /* current length of the word */

/* Expand the word until it has length >= n.  */
static void ensure_len(idx_t n)
{
    if (w == NULL) {
        /* start with the single symbol 1 */
        w = (seq_t*)malloc(1 * sizeof(seq_t));
        if (w == NULL) abort();
        w[0] = 1;
        w_len = 1;
    }

    while (w_len < n) {
        /* compute new length after one substitution */
        idx_t new_len = 0;
        for (idx_t i = 0; i < w_len; ++i) {
            seq_t v = w[i];
            /* each 0 or 1 expands to 2 symbols, 2 expands to 1 symbol */
            new_len += (v == 2) ? 1 : 2;
        }

        seq_t *neww = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (neww == NULL) abort();

        /* build the next word */
        idx_t pos = 0;
        for (idx_t i = 0; i < w_len; ++i) {
            seq_t v = w[i];
            if (v == 0) {          /* 0 → 10 */
                neww[pos++] = 1;
                neww[pos++] = 0;
            } else if (v == 1) {   /* 1 → 12 */
                neww[pos++] = 1;
                neww[pos++] = 2;
            } else {               /* 2 → 0   */
                neww[pos++] = 0;
            }
        }

        free(w);
        w = neww;
        w_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, safe fallback */
    ensure_len(n);
    return w[n - 1];
}
