/*
 * A284588  1-limiting word of the morphism 0->1, 1->0101.
 * https://oeis.org/A284588
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

/* Sequence A284588 – 1‑limiting word of the morphism
 * 0 → 1, 1 → 0101.  The value a(n) is the n‑th bit (1‑based) in the
 * 1‑word obtained after sufficiently many substitutions.  The
 * algorithm expands the word iteration by iteration until an odd
 * iteration reaches length at least n, then returns the stored bit. */

#include <stdlib.h>
#include <stdint.h>     /* for uint8_t */

seq_t a(idx_t n)
{
    /* for indices below the offset we return 0, the definition
       is relevant only from n ≥ 1. */
    if (n == 0) return 0;

    /* start with the initial word "0".  wlen counts its length. */
    uint8_t *word = (uint8_t*)malloc(1);
    word[0] = 0;
    size_t wlen = 1;

    /* depth counts the number of substitutions performed.  depth 0
       corresponds to the initial word, depth 1 after the first
       morphic image.  The 1‑limiting word stabilises after an odd
       substitution. */
    size_t depth = 0;

    while (1) {
        /* If we are on an odd iteration and the word already
           contains the desired position, return it. */
        if ((depth & 1U) && wlen >= (size_t)n) {
            seq_t result = (seq_t)word[(size_t)n - 1];
            free(word);
            return result;
        }

        /* Generate the next word by applying the morphism. */
        size_t new_cap = 4 * wlen;          /* each symbol may become 4 symbols */
        uint8_t *next = (uint8_t*)malloc(new_cap);
        size_t new_len = 0;

        for (size_t i = 0; i < wlen; ++i) {
            if (word[i] == 0) {            /* 0 → 1 */
                next[new_len++] = 1;
            } else {                       /* 1 → 0101 */
                next[new_len++] = 0;
                next[new_len++] = 1;
                next[new_len++] = 0;
                next[new_len++] = 1;
            }
        }

        free(word);
        word = next;
        wlen = new_len;
        ++depth;
    }
}
