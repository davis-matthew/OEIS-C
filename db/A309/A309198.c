/*
 * A309198  Fixed point of the morphism 1 -> 12, 2 -> 3, 3 -> 4, 4 -> 123.
 * https://oeis.org/A309198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9803, u16 n <= 9803, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309198: Fixed point of the morphism 1 → 12, 2 → 3, 3 → 4, 4 → 123.
   The algorithm expands the word until the required index is reached. */

#include <stdlib.h>

static seq_t *word = NULL;   /* buffer holding the generated word   */
static size_t  len  = 0;      /* number of symbols currently stored  */

/* Expand the word until it contains at least n+1 symbols. */
static void
ensure_length(idx_t n)
{
    if (len > n) return;                /* already long enough */

    /* Initialise with the seed word if this is the first call. */
    if (len == 0) {
        word = (seq_t *)malloc(sizeof(seq_t) * 2);
        word[0] = (seq_t)1;
        len    = 1;
    }

    /* Repeatedly apply the morphism until the required length is reached. */
    while (len <= n) {
        /* First compute the new length to allocate enough space. */
        size_t new_len = 0;
        for (size_t i = 0; i < len; ++i) {
            const seq_t v = word[i];
            if      (v == (seq_t)1) new_len += 2; /* 12   */
            else if (v == (seq_t)2) new_len += 1; /* 3    */
            else if (v == (seq_t)3) new_len += 1; /* 4    */
            else if (v == (seq_t)4) new_len += 3; /* 123  */
        }

        /* Allocate the new word. */
        seq_t *new_word = (seq_t *)malloc(sizeof(seq_t) * new_len);
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i) {
            const seq_t v = word[i];
            if      (v == (seq_t)1) {
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)2;
            } else if (v == (seq_t)2) {
                new_word[pos++] = (seq_t)3;
            } else if (v == (seq_t)3) {
                new_word[pos++] = (seq_t)4;
            } else if (v == (seq_t)4) {
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)2;
                new_word[pos++] = (seq_t)3;
            }
        }

        /* Replace the old buffer. */
        free(word);
        word = new_word;
        len  = new_len;
    }
}

seq_t a(idx_t n)
{
    ensure_length(n);
    return word[(size_t)n];
}
