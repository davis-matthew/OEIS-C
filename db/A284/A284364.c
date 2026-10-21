/*
 * A284364  Fixed point of the morphism 0->1, 1->101010.
 * https://oeis.org/A284364
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

/* Sequence A284364: a(n) is the nth symbol of the fixed point of the
   morphism 0 → 1, 1 → 101010, with a(1) = 1.  The implementation
   iteratively expands this morphism until the required length is
   reached.  */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of the generated word */
    static seq_t *word = NULL;
    static size_t word_len = 0;

    /* initialise with the seed word '1' (a(1)) */
    if (word_len == 0) {
        word = (seq_t *)malloc(sizeof(seq_t));
        if (!word) return (seq_t)0;          /* out of memory, return 0 */
        word[0] = 1;
        word_len = 1;
    }

    /* extend until we have at least n symbols */
    while (word_len < (size_t)n) {
        /* count zeros and ones in the current word */
        size_t zeros = 0, ones = 0;
        size_t i;
        for (i = 0; i < word_len; ++i) {
            if (word[i] == 0) ++zeros; else ++ones;
        }

        /* new length after applying the morphism to every symbol */
        size_t new_len = zeros + ones * 6;
        seq_t *new_word = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_word) return (seq_t)0;  /* out of memory */

        /* fill the new word */
        size_t p = 0;
        for (i = 0; i < word_len; ++i) {
            seq_t s = word[i];
            if (s == 0) {
                new_word[p++] = 1;
            } else { /* s == 1 */
                new_word[p++] = 1; new_word[p++] = 0;
                new_word[p++] = 1; new_word[p++] = 0;
                new_word[p++] = 1; new_word[p++] = 0;
            }
        }

        /* discard old word and keep the new one */
        free(word);
        word = new_word;
        word_len = new_len;
    }

    return word[n - 1];
}
