/*
 * A284746  Positions of 0 in A284745; complement of A191263.
 * https://oeis.org/A284746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A284746: positions of 0 in the fixed point of the morphism
 *
 * 0 → 01
 * 1 → 000
 *
 * The word is generated iteratively; the nth 0 is found by scanning
 * the generated word.  No negative indices are used, 1‑based positions
 * are returned as seq_t as required.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state: the current word, its length and the count of zeros seen */
    static uint8_t *word = NULL;   /* 0/1 bits of the word                */
    static size_t word_len  = 0;   /* number of bits currently stored     */
    static size_t zeros_cnt = 0;   /* total number of zeros in current word*/

    /* initialise the word with the seed 0 */
    if (word == NULL) {
        word = (uint8_t*)malloc(1);
        if (word == NULL) return 0;     /* out of memory – unlikely but safe */
        word[0]  = 0;
        word_len = 1;
        zeros_cnt = 1;                  /* the seed contains one zero       */
    }

    /* grow the word until we have at least n zeros */
    while (zeros_cnt < (size_t)n) {
        size_t old_len = word_len;
        /* worst‑case expansion length is old_len * 3 */
        size_t new_capacity = old_len * 3;

        uint8_t *new_word = (uint8_t*)malloc(new_capacity);
        if (new_word == NULL) return 0;   /* unlikely, but keep compile safety */

        size_t pos   = 0;          /* current position in new_word       */
        size_t zcnt  = 0;          /* zeros count in the new word       */

        for (size_t i = 0; i < old_len; ++i) {
            uint8_t bit = word[i];
            if (bit == 0) {
                /* 0 → 0 1 */
                new_word[pos++] = 0;
                new_word[pos++] = 1;
                zcnt += 1;
            } else {
                /* 1 → 0 0 0 */
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                zcnt += 3;
            }
        }

        /* replace the old word */
        free(word);
        word = new_word;
        word_len = pos;
        zeros_cnt = zcnt;
    }

    /* locate the nth zero in the current word */
    size_t target = (size_t)n;
    for (size_t i = 0; i < word_len; ++i) {
        if (word[i] == 0) {
            if (--target == 0) {
                /* positions are 1‑based */
                return (seq_t)(i + 1);
            }
        }
    }

    /* we should never reach this point */
    return 0;
}
