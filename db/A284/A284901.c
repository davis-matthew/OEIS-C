/*
 * A284901  Fixed point of the morphism 0 -> 01, 1 -> 1000.
 * https://oeis.org/A284901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth symbol in the fixed point of the morphism 0 → 01,
 *            1 → 1000. a(1) = 0, a(2) = 1, a(3) = 1, …
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is generated iteratively: start from the word "0"
   and repeatedly replace every 0 by 01 and every 1 by 1000 until the
   generated word has at least n symbols.  For n up to ~10 000 this
   is well within the time limits. */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* static cache of the generated word */
    static seq_t *word = NULL;      /* pointer to the word */
    static idx_t len   = 0;         /* current length of the word */
    static idx_t cap   = 0;         /* allocated capacity */

    /* initialise with the first symbol "0" on the first call */
    if (word == NULL) {
        cap = 1;
        word = (seq_t *)malloc(cap * sizeof(seq_t));
        word[0] = 0;
        len   = 1;
    }

    /* grow the word until it contains at least n symbols (1‑based) */
    while (len < n) {
        /* compute the length of the next word */
        idx_t new_len = 0;
        for (idx_t i = 0; i < len; ++i) {
            new_len += (word[i] == 0) ? 2 : 4;
        }

        /* reallocate if needed */
        if (new_len > cap) {
            idx_t new_cap = new_len;
            seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
            /* copy the old word */
            for (idx_t i = 0; i < len; ++i) {
                new_arr[i] = word[i];
            }
            free(word);
            word = new_arr;
            cap  = new_cap;
        }

        /* build the next word in-place in the (now large enough) buffer */
        idx_t pos = 0;          /* write position in the new buffer */
        for (idx_t i = 0; i < len; ++i) {
            if (word[i] == 0) {
                word[pos++] = 0;
                word[pos++] = 1;
            } else {
                word[pos++] = 1;
                word[pos++] = 0;
                word[pos++] = 0;
                word[pos++] = 0;
            }
        }
        len = new_len;
    }

    /* return the requested symbol (convert 1‑based n to 0‑based index) */
    return word[n - 1];
}
