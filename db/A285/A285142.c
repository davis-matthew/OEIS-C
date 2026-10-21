/*
 * A285142  1-limiting word of the morphism 0->10, 1->0010.
 * https://oeis.org/A285142
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

/* a(n) = the n-th bit (1‑indexed) of the 1‑limiting word for the morphism
 *   0 → 10
 *   1 → 0010
 * The word is obtained by starting with 0 and iterating the morphism.
 * After each iteration we get a finite binary word; every odd‑indexed
 * iteration gives a prefix of the limit word.  This function returns the
 * required bit for any n ≥ 1.  It keeps a static cache of the word up to
 * the largest required odd iteration. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below offset is irrelevant */
        return (seq_t)0;

    /* static local state */
    static uint8_t *word = NULL;      /* current word */
    static size_t word_len = 0;       /* its length */
    static unsigned iter = 0;         /* iteration number, starts at 0 */

    /* initialise with the initial word "0" */
    if (word == NULL) {
        word = (uint8_t *)malloc(1);
        word[0] = 0;
        word_len = 1;
        iter = 0;          /* even iteration */
    }

    /* iterate until we are at an odd iteration and the word is long
       enough to contain the n-th bit. */
    while (iter % 2 == 0 || word_len < (size_t)n) {
        /* each symbol expands to at most 4 symbols, so new length
           is at most word_len * 4. */
        size_t new_len = word_len * 4;
        uint8_t *new_word = (uint8_t *)malloc(new_len);
        size_t pos = 0;

        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == 0) {          /* 0 → 10 */
                new_word[pos++] = 1;
                new_word[pos++] = 0;
            } else {                     /* 1 → 0010 */
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                new_word[pos++] = 1;
                new_word[pos++] = 0;
            }
        }
        free(word);
        word = new_word;
        word_len = pos;
        ++iter;   /* move to next iteration (parity changes) */
    }

    /* return the n-th bit (1‑indexed). */
    return (seq_t)word[(size_t)n - 1];
}
