/*
 * A285530  0-limiting word of the morphism 0->11, 1-> 0100.
 * https://oeis.org/A285530
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

/* A285530: 0‑limiting word of the morphism 0->11, 1->0100.
   Computed by iteratively applying the induced substitution
   f(0)=01000100, f(1)=1101001111 until the word length
   reaches the requested index. */
#include <stdlib.h>
#include <string.h>

static const seq_t f0[8] = {
    /* 0 -> 01 00 01 00  */
    0,1,0,0,0,1,0,0
};
static const seq_t f1[10] = {
    /* 1 -> 11 01 00 1111 */
    1,1,0,1,0,0,1,1,1,1
};

/* State kept between calls: current prefix of the limit word. */
static seq_t *word = NULL;
static size_t word_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out-of‑range, but not used */
    /* Ensure we have an initial word of length 1 */
    if (word_len == 0) {
        word_len = 1;
        word = (seq_t *)malloc(word_len * sizeof(seq_t));
        if (!word) exit(1);
        word[0] = 0;
    }

    /* Grow the word until it is long enough */
    while (word_len < (size_t)n) {
        size_t new_len = 0;
        /* First compute total length of next iteration */
        for (size_t i = 0; i < word_len; ++i) {
            new_len += (word[i] == 0) ? 8 : 10;
        }

        /* Allocate new word */
        seq_t *new_word = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_word) exit(1);

        /* Expand each symbol */
        size_t pos = 0;
        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == 0) {
                memcpy(&new_word[pos], f0, 8 * sizeof(seq_t));
                pos += 8;
            } else {
                memcpy(&new_word[pos], f1, 10 * sizeof(seq_t));
                pos += 10;
            }
        }

        /* Replace old word */
        free(word);
        word = new_word;
        word_len = new_len;
    }

    /* Return the requested symbol (1‑based index). */
    return word[(size_t)(n - 1)];
}
