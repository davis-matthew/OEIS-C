/*
 * A285661  Fixed point of the morphism 0->11, 1->1100.
 * https://oeis.org/A285661
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

/* Fixed point of the morphism 0→11, 1→1100.  a(n) is the nth symbol (1‑indexed) of the infinite word obtained by iterating the morphism starting with the single symbol 0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t sz = 0;

    if (n == 0)
        return 0; /* offset is 1; callers should use n>=1 */

    /* initialise with the first symbol of the fixed point, which is 1 */
    if (seq == NULL) {
        seq = (seq_t *)malloc(sizeof(seq_t));
        seq[0] = 1;
        sz = 1;
    }

    /* expand until we have at least n symbols */
    while (sz < n) {
        /* worst case 1 expands to 4 symbols, 0 expands to 2 */
        idx_t new_sz_est = sz * 4;
        seq_t *new_seq = (seq_t *)malloc(new_sz_est * sizeof(seq_t));
        idx_t pos = 0;

        for (idx_t i = 0; i < sz; ++i) {
            seq_t v = seq[i];
            if (!v) {                 /* 0 -> 11 */
                new_seq[pos++] = 1;
                new_seq[pos++] = 1;
            } else {                  /* 1 -> 1100 */
                new_seq[pos++] = 1;
                new_seq[pos++] = 1;
                new_seq[pos++] = 0;
                new_seq[pos++] = 0;
            }
        }

        free(seq);
        seq = new_seq;
        sz = pos;                     /* actual new length */
    }

    return seq[n - 1];
}
