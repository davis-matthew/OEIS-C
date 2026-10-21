/*
 * A284954  0-limiting word of the morphism 0->10, 1-> 000.
 * https://oeis.org/A284954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth digit of the word obtained after the even iteration
 *            of the morphism 0→10, 1→000, starting from 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for out‑of‑range n */

    /* initial word (iteration 0) */
    idx_t cur_len = 1;
    char *word = (char*)malloc(cur_len);
    if (!word) return (seq_t)0;              /* allocation failure guard */
    word[0] = '0';
    idx_t iter = 0;                          /* 0 is even */

    /* generate until an even iteration has enough length to cover n */
    while (cur_len < n || (iter & 1)) {      /* stop when cur_len>=n and iter even */
        /* count ones to compute next length */
        idx_t ones = 0;
        for (idx_t i = 0; i < cur_len; ++i)
            if (word[i] == '1')
                ++ones;

        idx_t new_len = 2 * cur_len + ones;   /* 0→10 adds 2, 1→000 adds 3 characters */
        char *new_word = (char*)malloc(new_len);
        if (!new_word) {                      /* allocation failure guard */
            free(word);
            return (seq_t)0;
        }

        /* build next word */
        idx_t pos = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            if (word[i] == '0') {
                new_word[pos++] = '1';
                new_word[pos++] = '0';
            } else {                          /* '1' */
                new_word[pos++] = '0';
                new_word[pos++] = '0';
                new_word[pos++] = '0';
            }
        }

        free(word);
        word   = new_word;
        cur_len= new_len;
        ++iter;
    }

    seq_t result = (seq_t)(word[n - 1] - '0');   /* convert '0'/'1' to 0/1 */
    free(word);
    return result;
}
