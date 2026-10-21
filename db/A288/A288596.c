/*
 * A288596  Fixed point of the mapping 00->0101, 10->1000, starting with 00.
 * https://oeis.org/A288596
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

/* Fixed point of the morphism
 *   00 → 0101
 *   10 → 1000
 * starting with the word 00.
 * a(n) = the n‑th bit (1‑based) of the resulting infinite word. */
#include <stdint.h>
#include <stdlib.h>

static uint8_t *word = NULL;   /* current word in the iteration   */
static idx_t word_len = 0;      /* length of the current word      */

/* Replace the current word by its image under the morphism. */
static void next_word(void)
{
    /* Each matched pair expands 2 → 4 bits, so allocate a safe quota. */
    size_t new_cap = word_len * 4;
    uint8_t *next = (uint8_t *)malloc(new_cap);
    size_t j = 0;   /* index into the output buffer            */
    idx_t i;

    for (i = 0; i + 1 < word_len; ++i) {
        if (word[i] == 0 && word[i + 1] == 0) {          /* 00 → 0101 */
            next[j++] = 0; next[j++] = 1;
            next[j++] = 0; next[j++] = 1;
            ++i;                /* skip the second bit of the pair      */
        } else if (word[i] == 1 && word[i + 1] == 0) {   /* 10 → 1000 */
            next[j++] = 1; next[j++] = 0;
            next[j++] = 0; next[j++] = 0;
            ++i;
        } else {                                         /* copy single bit */
            next[j++] = word[i];
        }
    }
    /* Copy the last bit if it was not consumed by a matched pair. */
    if (!(word_len >= 2 &&
          ((word[word_len - 2] == 0 && word[word_len - 1] == 0) ||
           (word[word_len - 2] == 1 && word[word_len - 1] == 0)))) {
        next[j++] = word[word_len - 1];
    }

    free(word);
    word      = next;
    word_len  = (idx_t)j;   /* new word length */
}

/* Return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    /* Initialise the starting word on first call. */
    if (word == NULL) {
        word_len = 2;
        word = (uint8_t *)malloc(2);
        word[0] = 0;
        word[1] = 0;
    }
    /* Iterate until we have enough of the fixed point so that
       the n-th bit is certainly fixed. */
    while (word_len <= n) {
        next_word();
    }
    return (seq_t)word[n - 1];
}
