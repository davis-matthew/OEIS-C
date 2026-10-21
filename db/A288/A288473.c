/*
 * A288473  1-limiting word of the mapping 00->0101, 1->011, starting with 00.
 * https://oeis.org/A288473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   map_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1-limiting word of the mapping 00→0101, 1→011, starting with 00.
    The sequence a(n) is the n‑th bit of the infinite limit word obtained
    by repeatedly applying the mapping twice (odd iterates).  Implementation
    generates odd iterates until the required position is reached. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/map_word.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is defined for n≥1 */

    static seq_t *odd_word = NULL;         /* current odd iterate word */
    static size_t odd_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* First odd iterate: T(00) = 0101 */
        odd_word = (seq_t *)malloc(4 * sizeof(seq_t));
        odd_word[0] = (seq_t)0;
        odd_word[1] = (seq_t)1;
        odd_word[2] = (seq_t)0;
        odd_word[3] = (seq_t)1;
        odd_len = 4;
        initialized = true;
    }

    /* Grow the odd iterate until it contains the desired index. */
    while (odd_len < (size_t)n) {
        seq_t *even_word = NULL;
        size_t even_len = 0;
        map_word(odd_word, odd_len, &even_word, &even_len);

        seq_t *new_odd = NULL;
        size_t new_odd_len = 0;
        map_word(even_word, even_len, &new_odd, &new_odd_len);

        free(even_word);      /* free intermediate even iterate */
        free(odd_word);       /* free previous odd iterate */
        odd_word = new_odd;
        odd_len = new_odd_len;
    }

    return odd_word[n - 1];
}
