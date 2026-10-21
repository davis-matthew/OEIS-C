/*
 * A285305  Fixed point of the morphism 0 -> 10, 1 -> 1001.
 * https://oeis.org/A285305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth (1‑based) bit of the fixed point of the morphism
 *            0→10, 1→1001
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0, return 0 */

    static char *word = NULL;      /* current prefix of the infinite word */
    static size_t word_len = 0;    /* its length */

    /* initialise with the first two terms μ(0) = "10" */
    if (word == NULL) {
        word_len = 2;
        word     = (char*)malloc(word_len);
        if (!word) return (seq_t)0;          /* allocation failed */
        word[0] = '1';
        word[1] = '0';
    }

    /* expand until we have at least n terms */
    while (word_len < (size_t)n) {
        /* compute length of the next word */
        size_t new_len = 0;
        for (size_t i = 0; i < word_len; ++i)
            new_len += (word[i] == '0') ? 2 : 4;

        /* allocate new word */
        char *new_word = (char*)malloc(new_len);
        if (!new_word) return (seq_t)0;      /* allocation failed */

        /* apply the morphism */
        size_t pos = 0;
        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == '0') {            /* 0 → 10 */
                new_word[pos++] = '1';
                new_word[pos++] = '0';
            } else {                        /* 1 → 1001 */
                new_word[pos++] = '1';
                new_word[pos++] = '0';
                new_word[pos++] = '0';
                new_word[pos++] = '1';
            }
        }

        /* replace old word */
        free(word);
        word     = new_word;
        word_len = new_len;
    }

    /* return the nth bit (1‑based) */
    return (seq_t)(word[n - 1] == '1' ? 1 : 0);
}
