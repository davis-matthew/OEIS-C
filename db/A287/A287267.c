/*
 * A287267  1-limiting word of the morphism 0->11, 1->02, 2->0.
 * https://oeis.org/A287267
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

/* 1-limiting word of morphism 0->11, 1->02, 2->0 (odd iterates). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for n < 1 */

    static unsigned char *word = NULL;
    static idx_t word_len = 0;

    /* Initial word: f(0) = 11 */
    if (!word) {
        word_len = 2;
        word = (unsigned char *)malloc(word_len * sizeof(unsigned char));
        word[0] = 1;
        word[1] = 1;
    }

    /* Grow the word with the 2‑step morphism until it covers position n */
    while (word_len < n) {
        /* Each symbol expands to at most 4 symbols, so allocate 4×the current length. */
        unsigned char *next = (unsigned char *)malloc(word_len * 4 * sizeof(unsigned char));
        idx_t out = 0;
        for (idx_t i = 0; i < word_len; ++i) {
            unsigned char v = word[i];
            if (v == 0) {
                /* 0 -> 0202 */
                next[out++] = 0;
                next[out++] = 2;
                next[out++] = 0;
                next[out++] = 2;
            } else if (v == 1) {
                /* 1 -> 110 */
                next[out++] = 1;
                next[out++] = 1;
                next[out++] = 0;
            } else { /* v == 2 */
                /* 2 -> 11 */
                next[out++] = 1;
                next[out++] = 1;
            }
        }
        free(word);
        word = next;
        word_len = out;
    }

    return (seq_t)word[n - 1];
}
