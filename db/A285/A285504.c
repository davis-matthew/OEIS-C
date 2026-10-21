/*
 * A285504  1-limiting word of the morphism 0->11, 1-> 0011.
 * https://oeis.org/A285504
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

/*  This function returns the n‑th symbol of the infinite word obtained
    by iterating the morphism 0 -> 11, 1 -> 0011, taking the limit
    along odd iterations (the "1‑limiting word").
    The word is built iteratively by repeatedly applying the
    substitution
          0 -> 00110011     (8 symbols)
          1 -> 111100110011 (12 symbols)
    starting from the seed word 11.
    Each call enlarges the stored word only if necessary, so
    calls are independent of call order.  The algorithm is
    O(n) for a given n (n ≤ ~10000 in the validation tests). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of the currently generated word.
       It stores the “1‑limiting” word up to the maximum n seen so far.   */
    static unsigned char *word = NULL;     /* 0 or 1 values */
    static size_t word_len     = 0;          /* current length */
    static bool   initialized  = false;

    if (!initialized) {
        /* seed with the odd‑iteration word w1 = "11" */
        word_len = 2;
        word = (unsigned char*)malloc(word_len);
        word[0] = 1;
        word[1] = 1;
        initialized = true;
    }

    if ((idx_t)1 > n)        /* only defined for n ≥ 1, but guard anyway */
        return (seq_t)0;

    /* enlarge the word until it contains the requested index */
    while (word_len < (size_t)n) {
        /* compute length of the next iteration first */
        size_t new_len = 0;
        for (size_t i = 0; i < word_len; ++i)
            new_len += (word[i] == 0) ? 8 : 12;

        unsigned char *new_word = (unsigned char*)malloc(new_len);
        size_t pos = 0;

        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == 0) {
                /* 0 -> 00110011 */
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                new_word[pos++] = 1;
                new_word[pos++] = 1;
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                new_word[pos++] = 1;
                new_word[pos++] = 1;
            } else {
                /* 1 -> 111100110011 */
                new_word[pos++] = 1;
                new_word[pos++] = 1;
                new_word[pos++] = 1;
                new_word[pos++] = 1;
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                new_word[pos++] = 1;
                new_word[pos++] = 1;
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                new_word[pos++] = 1;
                new_word[pos++] = 1;
            }
        }

        free(word);
        word = new_word;
        word_len = new_len;
    }

    return (seq_t)word[(size_t)n - 1];
}
