/*
 * A285133  0-limiting word of the morphism 0->10, 1-> 0001.
 * https://oeis.org/A285133
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

/* 0-limiting word of the morphism 0->10, 1->0001.
   a(n) is the n-th symbol (0 or 1) of the infinite word obtained
   by iteratively applying the morphism to the initial symbol 0,
   but only on even iterations (the 0‑limiting word).  The word
   is generated until it has at least n symbols, ensuring the
   iteration count is even. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *word = NULL;      /* current word */
    static idx_t len  = 0;          /* length of current word */
    static idx_t iter = 0;          /* number of expansions performed */

    /* initialise the word with the single symbol 0 */
    if (word == NULL)
    {
        word = (seq_t *)malloc(sizeof(seq_t));
        if (!word) return (seq_t)-1;   /* allocation failed */
        word[0] = 0;
        len = 1;
        iter = 0;
    }

    /* grow the word until we have at least n symbols and the
       number of iterations is even (the 0‑limiting word) */
    while (len < n || (len >= n && (iter & 1)))
    {
        idx_t new_len = 0;
        for (idx_t i = 0; i < len; ++i)
            new_len += (word[i] == 0) ? 2 : 4;

        seq_t *new_word = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_word) return (seq_t)-1;   /* allocation failed */

        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i)
        {
            seq_t c = word[i];
            if (c == 0)       /* 0 -> 10 */
            {
                new_word[pos++] = 1;
                new_word[pos++] = 0;
            }
            else              /* 1 -> 0001 */
            {
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                new_word[pos++] = 1;
            }
        }

        free(word);
        word = new_word;
        len  = new_len;
        ++iter;   /* one more morphism application */
    }

    /* n is 1‑based */
    return word[n - 1];
}
