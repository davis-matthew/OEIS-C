/*
 * A285625  Fixed point of the morphism 0->11, 1->1010.
 * https://oeis.org/A285625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol (1-indexed) of the fixed point of * the
 *            morphism 0 → 11, 1 → 1010, starting from the seed word "1"
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static array that stores the current expansion of the word */
    static seq_t *word = NULL;
    static idx_t len   = 0;          /* current length of word */

    /* initialise with the seed word "1" */
    if (!word) {
        word = (seq_t*)malloc(sizeof(seq_t));
        if (!word) abort();          /* insufficient memory */
        word[0] = (seq_t)1;
        len    = 1;
    }

    /* grow the word until it contains at least n symbols */
    while (len < n) {
        /* compute how many symbols the next expansion will produce */
        idx_t new_len = 0;
        for (idx_t i = 0; i < len; ++i)
            new_len += (word[i] == (seq_t)0) ? 2 : 4;   /* 0 → 11; 1 → 1010 */

        /* allocate array for the expanded word */
        seq_t *new_word = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_word) abort();      /* insufficient memory */

        /* fill the expanded word according to the morphism rules */
        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (word[i] == (seq_t)0) {
                new_word[pos++] = (seq_t)1;   /* 0 → 11 */
                new_word[pos++] = (seq_t)1;
            } else {
                new_word[pos++] = (seq_t)1;   /* 1 → 1010 */
                new_word[pos++] = (seq_t)0;
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)0;
            }
        }

        /* replace old word with the new expansion */
        free(word);
        word = new_word;
        len  = new_len;
    }

    /* return the requested term (convert from 0- or 1-indexing) */
    return word[n - 1];
}
