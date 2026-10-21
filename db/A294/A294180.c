/*
 * A294180  The 3-symbol Pell word.
 * https://oeis.org/A294180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) from OEIS A294180 by iterative morphic expansion of the
   3‑symbol Pell word.  The sequence is defined as the unique fixed point
   of the morphism
            1 → 123
            2 → 123
            3 → 1
   starting from the seed 1. */

#include <stdlib.h>

static seq_t *word = NULL;   /* current word buffer      */
static idx_t word_len = 0;   /* current length of the word */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;       /* undefined for n==0, return 0 as sentinel */
    }

    const idx_t target = n - 1;   /* 0‑based target index */

    /* initialise the seed word "1" only once */
    if (word_len == 0) {
        word = (seq_t *)malloc(sizeof(seq_t));
        if (!word) return (seq_t)0;      /* allocation failure */
        word[0] = (seq_t)1;
        word_len = 1;
    }

    /* Expand until the word is long enough */
    while (word_len <= target) {
        /* Count occurrences of symbol 3 */
        idx_t count_3 = 0;
        for (idx_t i = 0; i < word_len; ++i) {
            if (word[i] == (seq_t)3) {
                ++count_3;
            }
        }

        /* Length after one morphic expansion:
           each 1 or 2 → 3 letters, each 3 → 1 letter */
        idx_t new_len = word_len * (idx_t)3 - count_3 * (idx_t)2;

        seq_t *new_word = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        if (!new_word) {
            return (seq_t)0;   /* allocation failure */
        }

        idx_t pos = 0;
        for (idx_t i = 0; i < word_len; ++i) {
            seq_t ch = word[i];
            if (ch == (seq_t)1 || ch == (seq_t)2) {
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)2;
                new_word[pos++] = (seq_t)3;
            } else {            /* ch == 3 */
                new_word[pos++] = (seq_t)1;
            }
        }

        free(word);
        word = new_word;
        word_len = new_len;
    }

    return word[target];
}
