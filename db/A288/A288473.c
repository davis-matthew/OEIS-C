/*
 * A288473  1-limiting word of the mapping 00->0101, 1->011, starting with 00.
 * https://oeis.org/A288473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9068 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Apply the mapping once to an input word.  The result is stored in *out
   and the new length is written to *out_len.  Memory is allocated with
   malloc and must be freed by the caller. */
static void map_word(const seq_t *in, size_t in_len, seq_t **out, size_t *out_len)
{
    size_t len = 0;

    /* First pass: determine output length. */
    for (size_t i = 0; i < in_len; ++i) {
        if (in[i] == (seq_t)0) {
            if (i + 1 < in_len && in[i + 1] == (seq_t)0) {
                len += 4;          /* 00 → 0101 */
                ++i;               /* skip the next 0 of the pair */
            } else {
                len += 1;          /* single 0 → 0 */
            }
        } else { /* in[i] == 1 */
            len += 3;              /* 1 → 011 */
        }
    }

    /* Allocate output buffer. */
    seq_t *buf = mem_new(seq_t, len);
    *out = buf;
    *out_len = len;

    /* Second pass: fill output buffer. */
    size_t j = 0;
    for (size_t i = 0; i < in_len; ++i) {
        if (in[i] == (seq_t)0) {
            if (i + 1 < in_len && in[i + 1] == (seq_t)0) {
                /* 00 → 0101 */
                buf[j++] = (seq_t)0;
                buf[j++] = (seq_t)1;
                buf[j++] = (seq_t)0;
                buf[j++] = (seq_t)1;
                ++i; /* skip the next 0 of the pair */
            } else {
                buf[j++] = (seq_t)0;
            }
        } else { /* in[i] == 1 */
            buf[j++] = (seq_t)0;
            buf[j++] = (seq_t)1;
            buf[j++] = (seq_t)1;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is defined for n≥1 */

    static seq_t *odd_word = NULL;         /* current odd iterate word */
    static size_t odd_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* First odd iterate: T(00) = 0101 */
        odd_word = mem_new(seq_t, 4);
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

        mem_free(even_word);      /* free intermediate even iterate */
        mem_free(odd_word);       /* free previous odd iterate */
        odd_word = new_odd;
        odd_len = new_odd_len;
    }

    return odd_word[n - 1];
}
