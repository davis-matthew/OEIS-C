/*
 * A285255  0-limiting word of the morphism 0->10, 1-> 0110.
 * https://oeis.org/A285255
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

/* 0-limiting word of morphism 0->10, 1->0110.
   a(n) is the nth (1‑based) symbol of the limit word obtained
   after an even number of substitutions starting from the symbol 0. */

#include <stdlib.h>
#include <stdint.h>

/* stored word (only 0 or 1) and its current length */
static uint8_t *word = NULL;
static size_t word_len = 0;
/* number of substitutions that have been applied to get 'word' */
static idx_t word_iter = 0;

/* apply one substitution step to the current word */
static void apply_morphism(void)
{
    size_t cur_len = word_len;
    /* compute length of the word after next morphism application */
    size_t new_len = 0;
    for (size_t i = 0; i < cur_len; ++i)
        new_len += (word[i] == 0) ? 2 : 4;

    /* allocate new buffer */
    uint8_t *new_word = (uint8_t*)malloc(sizeof(uint8_t) * new_len);
    if (new_word == NULL) {
        /* allocation failure – abort (not expected for allowed n) */
        exit(EXIT_FAILURE);
    }

    /* fill new buffer with the morphism */
    size_t pos = 0;
    for (size_t i = 0; i < cur_len; ++i) {
        if (word[i] == 0) {
            new_word[pos++] = 1;
            new_word[pos++] = 0;
        } else {
            new_word[pos++] = 0;
            new_word[pos++] = 1;
            new_word[pos++] = 1;
            new_word[pos++] = 0;
        }
    }

    free(word);
    word = new_word;
    word_len = new_len;
    word_iter++;
}

/* grow the word until it contains at least n symbols and
   the number of substitutions performed is even (0‑limiting). */
static void ensure_len(idx_t n)
{
    if (n == 0) return;              /* undefined, but harmless */

    /* initialise seed if necessary */
    if (word == NULL) {
        word = (uint8_t*)malloc(sizeof(uint8_t));
        word[0] = 0;
        word_len = 1;
        word_iter = 0;
    }

    /* apply morphism until the word is long enough */
    while ((idx_t)word_len < n) {
        apply_morphism();
    }

    /* ensure even number of substitutions (0‑limiting word) */
    if ((word_iter & 1) != 0) {
        apply_morphism();           /* one more step makes it even */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined; return 0 */
    ensure_len(n);
    return (seq_t)word[(size_t)(n - 1)];
}
