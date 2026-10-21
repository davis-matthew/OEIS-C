/*
 * A285663  Positions of 1 in A285661; complement of A285662.
 * https://oeis.org/A285663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285663: positions of 1 in the infinite word obtained by
   iteratively applying the morphism
          1 → 1100
          0 → 11
   starting with the single symbol 1.  For a given n (1‑based),
   the function returns the index of the nth occurrence of 1 in that word. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* unused – offset is 1 */

    /* Raise simple alias for loop variables. */
    size_t nn = (size_t)n;

    /* Persistent state between calls. */
    static uint8_t *word    = NULL;   /* current word (bytes 0/1)          */
    static size_t  word_len = 0;      /* length of current word            */
    static size_t  ones_cnt = 0;      /* total number of 1's in current word */

    /* First call – initialise with the single symbol 1. */
    if (word == NULL) {
        word    = (uint8_t *)malloc(1);
        word[0] = 1;
        word_len = 1;
        ones_cnt = 1;
    }

    /* Expand the word until it contains at least nn ones. */
    while (ones_cnt < nn) {
        /* The maximum possible length after expansion is 4*word_len. */
        size_t new_cap = word_len * 4 + 16;    /* small safety headroom */
        uint8_t *new_word = (uint8_t *)malloc(new_cap);
        size_t new_len = 0;
        size_t new_ones = 0;

        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == 1) {                /* 1 → 1100 */
                new_word[new_len++] = 1; new_ones++;
                new_word[new_len++] = 1; new_ones++;
                new_word[new_len++] = 0;
                new_word[new_len++] = 0;
            } else {                           /* 0 → 11 */
                new_word[new_len++] = 1; new_ones++;
                new_word[new_len++] = 1; new_ones++;
            }
        }

        free(word);
        word    = new_word;
        word_len = new_len;
        ones_cnt = new_ones;
    }

    /* Scan the word to find the nth 1 (1‑based index). */
    size_t cnt = 0;
    for (size_t i = 0; i < word_len; ++i) {
        if (word[i] == 1) {
            ++cnt;
            if (cnt == nn)
                return (seq_t)(i + 1);        /* positions are 1‑based */
        }
    }

    /* Control never reaches here for valid n. */
    return (seq_t)0;
}
