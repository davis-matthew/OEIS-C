/*
 * A284653  0-limiting word of the morphism 0 -> 1, 1 -> 0110.
 * https://oeis.org/A284653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9884 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0‑limiting word of the morphism 0 → 1, 1 → 0110
 * The sequence consists of the fixed point of the substitution that
 * starts with 0.  It is obtained by iterating the substitution twice
 * (even number of iterations) until the prefix of length n stops
 * changing.  */
#include <stdlib.h>
#include "../utils/mem.h"

static void expand_one(const seq_t *in, idx_t inlen, seq_t **out, idx_t *outlen)
{
    /* Upper bound on the length after one expansion: 4 × inlen */
    idx_t est = inlen * 4;
    seq_t *w = mem_new(seq_t, est);
    if (w == NULL) {
        *out = NULL;
        *outlen = 0;
        return;
    }
    idx_t pos = 0;
    for (idx_t i = 0; i < inlen; ++i) {
        if (in[i] == 0) {
            w[pos++] = (seq_t)1;          /* 0 → 1   */
        } else {
            w[pos++] = (seq_t)0;          /* 1 → 0110 */
            w[pos++] = (seq_t)1;
            w[pos++] = (seq_t)1;
            w[pos++] = (seq_t)0;
        }
    }
    *out = w;
    *outlen = pos;
}

seq_t a(idx_t n)
{
    static seq_t *even_word = NULL;   /* current even‑iterate word */
    static idx_t even_len = 0;        /* its length */

    if (n == 0)            /* safety for offset 1 */
        return 0;

    /* initialise with the 0‑word (iteration 0) */
    if (even_word == NULL) {
        even_len = 1;
        even_word = mem_new(seq_t, even_len);
        if (even_word == NULL)
            return 0;                  /* allocation failure – unlikely */
        even_word[0] = (seq_t)0;
    }

    /* If the requested index is already covered by the current
     * fixed‑point prefix, just return it.  The current word is known
     * to be the fixed point for its whole length, because we stop
     * expanding only after the prefix stopped changing. */
    if (n <= even_len)
        return even_word[n - 1];

    /* enlarge the stored prefix until the n-th symbol stabilises */
    while (1) {
        seq_t *old_word = even_word;
        idx_t oldlen = even_len;

        /* first expansion */
        seq_t *tmp;
        idx_t tmplen;
        expand_one(old_word, oldlen, &tmp, &tmplen);

        /* second expansion */
        seq_t *next_word;
        idx_t nextlen;
        expand_one(tmp, tmplen, &next_word, &nextlen);

        /* compare prefixes of length n */
        int same = 0;
        if (oldlen >= n && nextlen >= n) {
            same = 1;
            for (idx_t i = 0; i < n; ++i) {
                if (old_word[i] != next_word[i]) {
                    same = 0;
                    break;
                }
            }
        }

        mem_free(old_word);
        mem_free(tmp);

        if (same) {                     /* stable prefix reached */
            even_word = next_word;
            even_len = nextlen;
            break;
        }

        /* otherwise continue expanding from the new even word */
        even_word = next_word;
        even_len = nextlen;
    }

    return even_word[n - 1];
}
