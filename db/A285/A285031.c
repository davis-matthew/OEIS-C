/*
 * A285031  0-limiting word of the morphism 0->10, 1-> 001.
 * https://oeis.org/A285031
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

/* Computes a(n), the nth term of A285031: the 0‑limiting word of the
   morphism 0 → 10, 1 → 001.  The word is obtained by iterating the
   morphism until the length reaches at least n and the number of
   iterations is even; the nth symbol (1‑based) is returned. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined below the OEIS offset; return 0 safely. */
    if (n < 1)
        return 0;

    /* Start with the seed word "0". */
    size_t len = 1;
    uint8_t *word = (uint8_t *)malloc(len);
    if (!word)
        return 0;                 /* allocation failure – unlikely */
    word[0] = 0;

    /* Keep track of how many iterations have been performed. */
    size_t step = 0;

    /* Iterate until length ≥ n and step is even (0‑limiting word). */
    while (len < (size_t)n || (step & 1)) {
        /* The next word can be at most 3 times longer (1→001, 0→10). */
        size_t max_len = len * 3;
        uint8_t *next = (uint8_t *)malloc(max_len);
        if (!next)
            break;                /* allocation failure – unlikely */

        size_t new_len = 0;
        for (size_t i = 0; i < len; ++i) {
            if (word[i] == 0) {   /* 0 → 1 0 */
                next[new_len++] = 1;
                next[new_len++] = 0;
            } else {              /* 1 → 0 0 1 */
                next[new_len++] = 0;
                next[new_len++] = 0;
                next[new_len++] = 1;
            }
        }

        free(word);
        word = next;
        len  = new_len;
        ++step;
    }

    /* Retrieve the (n‑1)st symbol (0‑based). */
    size_t idx = (size_t)(n - 1);
    seq_t result = (seq_t)word[idx];

    free(word);
    return result;
}
