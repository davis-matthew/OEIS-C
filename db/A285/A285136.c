/*
 * A285136  1-limiting word of the morphism 0->10, 1-> 0001.
 * https://oeis.org/A285136
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

/* Sequence A285136: nth term of the 1‑limiting word of the morphism
   0 → 10, 1 → 0001.
   The word is obtained by starting with the single symbol 1 and
   iterating the two‑step morphism f = morph ∘ morph, where
   f(0) = 000110  (length 6) and f(1) = 1010100001 (length 10).
   The algorithm builds the word until it reaches the desired index n
   and returns the n‑th symbol (1‑based). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Work with a native-sized type for array indices and lengths. */
    size_t N = (size_t)n; /* n is small (≤ 10 000 in the tests) */

    /* Initial word: 1 */
    uint8_t *cur = (uint8_t *)malloc(N);
    if (!cur)
        return (seq_t)0;   /* allocation failure – unlikely for N ≤ 10 000 */
    cur[0] = 1;
    size_t cur_len = 1;

    /* Fixed expansions for the two‑step morphism. */
    const uint8_t f0[6] = {0, 0, 0, 1, 1, 0};
    const uint8_t f1[10] = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1};

    while (cur_len < N) {
        uint8_t *next = (uint8_t *)malloc(N);
        if (!next) {
            free(cur);
            return (seq_t)0;
        }
        size_t nxt_len = 0;

        for (size_t i = 0; i < cur_len && nxt_len < N; ++i) {
            uint8_t ch = cur[i];
            if (ch == 0) {
                for (size_t k = 0; k < 6 && nxt_len < N; ++k)
                    next[nxt_len++] = f0[k];
            } else {  /* ch == 1 */
                for (size_t k = 0; k < 10 && nxt_len < N; ++k)
                    next[nxt_len++] = f1[k];
            }
        }

        free(cur);
        cur = next;
        cur_len = nxt_len;
    }

    uint8_t result = cur[N - 1];
    free(cur);
    return (seq_t)result;
}
