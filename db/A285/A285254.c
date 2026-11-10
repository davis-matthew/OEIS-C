/*
 * A285254  Positions of 1 in A285252; complement of A285253.
 * https://oeis.org/A285254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2n-1 if A285252(2n-1) = 1, else 2n (each pair of A285252
 *            is 10 or 01)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9618 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A285252_a(idx_t n)
{
    static seq_t *word = NULL;
    static idx_t  len  = 0;

    if (len == 0) {
        word = mem_new(seq_t, 2);
        word[0] = (seq_t)1;
        word[1] = (seq_t)0;
        len   = 2;
    }

    static const seq_t MAP0[6] = { (seq_t)0,(seq_t)1,(seq_t)0,
                                   (seq_t)1,(seq_t)1,(seq_t)0 };
    static const seq_t MAP1[12] = { (seq_t)1,(seq_t)0,(seq_t)0,
                                   (seq_t)1,(seq_t)0,(seq_t)1,
                                   (seq_t)1,(seq_t)0,(seq_t)0,
                                   (seq_t)1,(seq_t)0,(seq_t)1 };

    while (len < n) {
        idx_t zeros = 0, ones = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (word[i] == (seq_t)0) ++zeros;
            else ++ones;
        }
        idx_t new_len = zeros * 6 + ones * 12;

        seq_t *new_word = mem_new(seq_t, new_len);
        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (word[i] == (seq_t)0) {
                for (int k = 0; k < 6; ++k) new_word[pos++] = MAP0[k];
            } else {
                for (int k = 0; k < 12; ++k) new_word[pos++] = MAP1[k];
            }
        }

        mem_free(word);
        word = new_word;
        len  = new_len;
    }

    return word[n - 1];
}

seq_t a(idx_t n)
{
    seq_t v = A285252_a(2 * n - 1);
    if (v == (seq_t)1) return (seq_t)n * 2 - 1;
    return (seq_t)n * 2;
}
