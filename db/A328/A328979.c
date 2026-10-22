/*
 * A328979  Trajectory of 0 under repeated application of the morphism 0 ->
 *          0010, 1 -> 1010.
 * https://oeis.org/A328979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth digit of the fixed point of the morphism 0→0010,
 *            1→1010 (trajectory of 0 under repeated application)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the nth term (1‑indexed) of the infinite word
       obtained by iteratively applying the morphism:
           0 → 0 0 1 0
           1 → 1 0 1 0
       The word starts with 0 and continues forever. */

    if (n == 0) /* undefined for n<1; return 0 as safe fallback */
        return 0;

    idx_t len = 1;
    seq_t *w = (seq_t *)malloc(sizeof(seq_t));
    if (!w) return 0;          /* allocation failure – unlikely for n≤10000 */
    w[0] = 0;                   /* initial symbol 0 */

    while (len < n) {
        idx_t newlen = len * 4;
        seq_t *w2 = (seq_t *)malloc(newlen * sizeof(seq_t));
        if (!w2) {                 /* allocation failure */
            free(w);
            return 0;
        }
        /* Expand each symbol according to the morphism */
        for (idx_t i = 0, j = 0; i < len; ++i) {
            seq_t v = w[i];
            if (v == 0) {
                w2[j++] = 0;
                w2[j++] = 0;
                w2[j++] = 1;
                w2[j++] = 0;
            } else { /* v == 1 */
                w2[j++] = 1;
                w2[j++] = 0;
                w2[j++] = 1;
                w2[j++] = 0;
            }
        }
        free(w);
        w = w2;
        len = newlen;
    }

    seq_t result = w[n - 1];
    free(w);
    return result;
}
