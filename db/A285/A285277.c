/*
 * A285277  1-limiting word of the morphism 0->10, 1-> 0111.
 * https://oeis.org/A285277
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

/*
 * Formula: a(n) is the nth symbol of the 1‑limiting word of the morphism
 * 0 → 10, 1 → 0111.  The word is obtained by repeatedly applying the
 * morphism to the initial word containing the single symbol 0 until
 * the first symbol becomes 1 (which occurs after an odd number of
 * iterations) and the length of the word is at least n.  The sequence
 * values are 0 or 1.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* current word as 0/1 values, index 0 … len-1   */
    uint8_t *w = (uint8_t *)malloc(sizeof(uint8_t) * 1);
    if (!w) return (seq_t)0;           /* allocation failure */
    w[0] = 0;                          /* start word: "0" */
    idx_t len = 1;                      /* current length          */
    idx_t it  = 0;                      /* iteration counter       */

    /* iterate until we have an odd iteration count (first symbol = 1)
     * and the word length is at least n.                           */
    while (len < n || (it & 1) == 0) {
        /* determine the length of the next word */
        idx_t new_len = 0;
        for (idx_t i = 0; i < len; ++i) {
            new_len += (w[i] == 0) ? 2 : 4;
        }

        /* build the next word */
        uint8_t *nw = (uint8_t *)malloc(sizeof(uint8_t) * new_len);
        if (!nw) {
            free(w);
            return (seq_t)0;           /* allocation failure */
        }
        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (w[i] == 0) {           /* 0 → 1 0 */
                nw[pos++] = 1;
                nw[pos++] = 0;
            } else {                   /* 1 → 0 1 1 1 */
                nw[pos++] = 0;
                nw[pos++] = 1;
                nw[pos++] = 1;
                nw[pos++] = 1;
            }
        }

        free(w);
        w = nw;
        len = new_len;
        ++it;
    }

    /* at this point w[n-1] contains the desired bit */
    seq_t result = (seq_t)w[n - 1];
    free(w);
    return result;
}
