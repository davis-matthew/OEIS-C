/*
 * A285533  1-limiting word of the morphism 0->11, 1-> 0100.
 * https://oeis.org/A285533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n'th bit (1‑based) of the infinite word obtained * by
 *            iterating the morphism 0→11, 1→0100 starting from the single *
 *            symbol 1. The word evolves as 1 → 1101001111 →
 *            11010011111101001111 … * The function expands the current known
 *            word until its length ≥ n and * returns word[n‑1]. It work...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Blocks for the second iterate of the morphism (φ²). */
static const seq_t BLOCK1[10] = {
    1, 1, 0, 1, 0, 0, 1, 1, 1, 1
};
static const seq_t BLOCK0[8]  = {
    0, 1, 0, 0, 0, 1, 0, 0
};

#ifdef __cplusplus
static std::vector<seq_t> word;
static bool word_inited = false;
#else
static seq_t *word = NULL;  /* current word */
static size_t word_len = 0; /* length in elements */
#endif

/* Expand the word until its length is at least n. */
static void
ensure_length(idx_t n)
{
    if (n <= 1) {
#ifdef __cplusplus
        if (!word_inited) {
            word_inited = true;
            word.push_back(1);
        }
#else
        if (word == NULL) {
            word = (seq_t *)malloc(sizeof(seq_t));
            if (word == NULL) abort();
            word[0] = 1;
            word_len = 1;
        }
#endif
        return;
    }

#ifdef __cplusplus
    if (!word_inited) {
        word_inited = true;
        word.push_back(1);
    }
    while ((size_t)word.size() < (size_t)n) {
        size_t new_len = 0;
        for (size_t i = 0; i < word.size(); ++i) {
            new_len += (word[i] == 0 ? 8 : 10);
        }
        std::vector<seq_t> tmp;
        tmp.reserve(new_len);
        for (size_t i = 0; i < word.size(); ++i) {
            if (word[i] == 0) {
                for (int k = 0; k < 8; ++k) tmp.push_back(BLOCK0[k]);
            } else {
                for (int k = 0; k < 10; ++k) tmp.push_back(BLOCK1[k]);
            }
        }
        word.swap(tmp);
    }
#else
    if (word == NULL) {
        word = (seq_t *)malloc(sizeof(seq_t));
        if (word == NULL) abort();
        word[0] = 1;
        word_len = 1;
    }
    while (word_len < (size_t)n) {
        size_t new_len = 0;
        for (size_t i = 0; i < word_len; ++i) {
            new_len += (word[i] == 0 ? 8 : 10);
        }
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (tmp == NULL) abort();
        size_t pos = 0;
        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == 0) {
                for (int k = 0; k < 8; ++k) tmp[pos++] = BLOCK0[k];
            } else {
                for (int k = 0; k < 10; ++k) tmp[pos++] = BLOCK1[k];
            }
        }
        free(word);
        word = tmp;
        word_len = new_len;
    }
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n = 0, return 0 */
    ensure_length(n);
#ifdef __cplusplus
    return word[(size_t)(n-1)];
#else
    return word[(size_t)(n-1)];
#endif
}
