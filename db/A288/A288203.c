/*
 * A288203  Fixed point of the mapping 00->0010, 1->010, starting with 00.
 * https://oeis.org/A288203
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

/* Implements OEIS A288203: the fixed point of the morphism
   00 -> 0010, 1 -> 010, starting from the word "00".
   The function returns the (n‑th) symbol of this infinite word
   (1‑based indexing).  The word is generated on demand and
   memoized between calls. */
#include <stdlib.h>
#include <stdint.h>

static uint8_t *word = NULL;   /* current word buffer (0/1)      */
static idx_t   word_len = 0;   /* number of symbols in word     */

/* Return the n-th symbol of the fixed point word. */
seq_t a(idx_t n)
{
    /* initialize word as "00" on first call */
    if (word_len == 0) {
        word_len = 2;
        word = (uint8_t*)malloc(2 * sizeof(uint8_t));
        word[0] = word[1] = 0;
    }

    /* expand the word until it covers the desired index n */
    while (word_len < n) {
        /* Allocate a new buffer that is at least 4× larger (worst case) */
        size_t new_cap = (size_t)(word_len * 4 + 10);
        uint8_t *next = (uint8_t*)malloc(new_cap);

        idx_t i = 0, j = 0;
        while (i < word_len) {
            if (i + 1 < word_len && word[i] == 0 && word[i + 1] == 0) {
                /* "00" → "0010" */
                next[j++] = 0;
                next[j++] = 0;
                next[j++] = 1;
                next[j++] = 0;
                i += 2;
            } else if (word[i] == 1) {
                /* "1"  → "010" */
                next[j++] = 0;
                next[j++] = 1;
                next[j++] = 0;
                i += 1;
            } else { /* word[i] == 0 (single 0) */
                next[j++] = 0;
                i += 1;
            }
        }

        free(word);
        word = next;
        word_len = j;
    }

    return (seq_t)word[n - 1];
}
