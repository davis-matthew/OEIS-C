/*
 * A285252  1-limiting word of the morphism 0->10, 1-> 0101.
 * https://oeis.org/A285252
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

/* Computes the 1‑limiting word of the morphism
   0 → 10, 1 → 0101 by repeatedly applying the two‑step
   replacement until the stored word is long enough.
   The returned value is the n-th term (1‑based) of OEIS
   A285252. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *word = NULL;
    static idx_t  len  = 0;

    /* Initial 1‑limiting word after the first odd iteration:
       0 → 10  gives 10.  Store as [1,0]. */
    if (len == 0) {
        word = (seq_t*)malloc(2 * sizeof(seq_t));
        word[0] = (seq_t)1;
        word[1] = (seq_t)0;
        len   = 2;
    }

    /* Two‑step replacement mapping: 0→010110 (6 symbols),
       1→100101100101 (12 symbols). */
    static const seq_t MAP0[6] = { (seq_t)0,(seq_t)1,(seq_t)0,
                                   (seq_t)1,(seq_t)1,(seq_t)0 };
    static const seq_t MAP1[12] = { (seq_t)1,(seq_t)0,(seq_t)0,
                                   (seq_t)1,(seq_t)0,(seq_t)1,
                                   (seq_t)1,(seq_t)0,(seq_t)0,
                                   (seq_t)1,(seq_t)0,(seq_t)1 };

    while (len < n) {
        idx_t zeros = 0, ones = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (word[i] == (seq_t)0) ++zeros;
            else ++ones;
        }
        idx_t new_len = zeros * 6 + ones * 12;

        seq_t *new_word = (seq_t*)malloc(new_len * sizeof(seq_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (word[i] == (seq_t)0) {
                for (int k = 0; k < 6; ++k) new_word[pos++] = MAP0[k];
            } else {
                for (int k = 0; k < 12; ++k) new_word[pos++] = MAP1[k];
            }
        }

        free(word);
        word = new_word;
        len  = new_len;
    }

    return word[n - 1];
}
