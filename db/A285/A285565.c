/*
 * A285565  0-limiting word of the morphism 0->11, 1-> 0110.
 * https://oeis.org/A285565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   0‑limiting word of the morphism 0 → 11, 1 → 0110 (the
 *            even‑iteration limit, i.e. the fixed point of the doubled
 *            morphism)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static state: current word and its length. */
    static seq_t *word = NULL;
    static size_t len   = 0;

    /* Initial word: the seed 0. */
    if (len == 0)
    {
        word = (seq_t*)malloc(sizeof(seq_t));
        word[0] = 0;
        len    = 1;
    }

    /* Morphism φ² blocks: 0 → 01100110, 1 → 110110011011. */
    static const seq_t block0[8]  = {0,1,1,0,0,1,1,0};
    static const seq_t block1[12] = {1,1,0,1,1,0,0,1,1,0,1,1};

    /* Grow the word until it contains the requested index. */
    while (len < (size_t)n)
    {
        /* Count zeros and ones in the current word. */
        size_t zeros = 0, ones = 0;
        for (size_t i = 0; i < len; ++i)
            (word[i] == 0) ? ++zeros : ++ones;

        /* Compute length of the next word. */
        size_t newLen = zeros * 8 + ones * 12;
        seq_t *newWord = (seq_t*)malloc(newLen * sizeof(seq_t));

        /* Expand each symbol. */
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i)
        {
            if (word[i] == 0)
            {
                for (size_t j = 0; j < 8; ++j)
                    newWord[pos++] = block0[j];
            }
            else /* word[i] == 1 */
            {
                for (size_t j = 0; j < 12; ++j)
                    newWord[pos++] = block1[j];
            }
        }

        /* Replace old word. */
        free(word);
        word = newWord;
        len  = newLen;
    }

    /* Return the nth symbol (1‑indexed). */
    return word[(size_t)n - 1];
}
