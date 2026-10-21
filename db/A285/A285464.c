/*
 * A285464  0-limiting word of the morphism 0->11, 1-> 0001.
 * https://oeis.org/A285464
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

/* 0-limiting word of the morphism 0->11, 1->0001.
   Computes the nth (1‑based) symbol by iteratively expanding
   the word until the wanted position is within an even‑iteration
   limit word. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 1, safe fallback */
        return (seq_t)0;

    const size_t pos = (size_t)(n - 1);   /* 0‑based index */

    /* static state keeps generated word and iteration count between calls */
    static seq_t *word = NULL;
    static size_t  word_len = 0;
    static unsigned int iter = 0;         /* number of morphism iterations applied */

    /* initialise word from the seed "0" */
    if (word == NULL) {
        word = (seq_t *)malloc(sizeof(seq_t));
        word[0] = (seq_t)0;
        word_len = 1;
        iter = 0;
    }

    /* generate further iterations until an even iteration has length > pos */
    while (word_len <= pos || (iter % 2U) == 1U) {
        size_t zeros = 0, ones = 0;
        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == 0)
                ++zeros;
            else
                ++ones;
        }

        const size_t new_len = zeros * 2 + ones * 4;
        seq_t *next = (seq_t *)malloc(new_len * sizeof(seq_t));

        size_t p = 0;
        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == 0) {           /* 0 → 1 1 */
                next[p++] = (seq_t)1;
                next[p++] = (seq_t)1;
            } else {                      /* 1 → 0 0 0 1 */
                next[p++] = (seq_t)0;
                next[p++] = (seq_t)0;
                next[p++] = (seq_t)0;
                next[p++] = (seq_t)1;
            }
        }

        free(word);
        word = next;
        word_len = new_len;
        ++iter;
    }

    return word[pos];
}
