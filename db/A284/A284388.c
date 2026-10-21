/*
 * A284388  0-limiting word of the morphism 0 -> 1, 1 -> 001.
 * https://oeis.org/A284388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth symbol of the 0‑limiting word of the morphism 0 → 1,
 *            1 → 001, iterated an even number of times (equivalently, apply
 *            the composite morphism ψ: 0 → 001, 1 → 11001)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   allocate_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/allocate_seq_array.h"

/* Helpers that work in both C and C++ */

static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Cached even‑iteration word (0‑limiting word) */
static seq_t *word = NULL;
static idx_t  word_len = 0;

/* Grow the cached word until it contains at least n symbols */
static void ensure_up_to(idx_t n)
{
    if (n <= word_len)                    /* already sufficient */
        return;

    /* initialise with the seed symbol 0 (zero‑iteration word) */
    if (!word) {
        word_len = 1;
        word     = allocate_seq_array(1);
        word[0]  = (seq_t)0;
        return;
    }

    /* iteratively apply the composite morphism ψ: 0→001, 1→11001 */
    while (word_len < n) {
        idx_t old_len = word_len;
        idx_t new_len = 0;

        /* compute the length of the next even‑iteration word */
        for (idx_t i = 0; i < old_len; ++i)
            new_len += (word[i] == (seq_t)0) ? 3 : 5;

        /* allocate buffer for the new word */
        seq_t *new_word = allocate_seq_array(new_len);
        if (!new_word)          /* allocation failed – give up silently */
            return;

        /* build the new word from the old one using ψ */
        idx_t pos = 0;
        for (idx_t i = 0; i < old_len; ++i) {
            if (word[i] == (seq_t)0) {             /* 0 → 001 */
                new_word[pos++] = (seq_t)0;
                new_word[pos++] = (seq_t)0;
                new_word[pos++] = (seq_t)1;
            } else {                               /* 1 → 11001 */
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)0;
                new_word[pos++] = (seq_t)0;
                new_word[pos++] = (seq_t)1;
            }
        }

        /* replace the old word */
        free_seq_array(word);
        word     = new_word;
        word_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                        /* undefined for n<1; return 0 */
        return (seq_t)0;

    ensure_up_to(n);
    return word[n - 1];
}
