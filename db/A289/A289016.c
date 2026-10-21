/*
 * A289016  Fixed point of the mapping 00->0010, 01->010, 10->000, starting
 *          with 00.
 * https://oeis.org/A289016
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

/* Sequence A289016: the n‑th term is the n‑th bit (1‑based) of the
   fixed point of the mapping
      00 → 0010
      01 → 010
      10 → 000
   applied to non‑overlapping adjacent pairs, starting from 00. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The first two bits are always 0 */
    if (n <= 2) return (seq_t)0;

    /* Start with the word 00 */
    idx_t src_len = 2;
    unsigned char *src = (unsigned char *)malloc(src_len * sizeof(unsigned char));
    if (!src) return (seq_t)0;            /* allocation failure – fall back */
    src[0] = 0; src[1] = 0;

    /* Keep expanding until we have at least n bits */
    while (src_len < n) {
        /* Each pair produces at most 4 bits; pre‑allocate safely */
        idx_t dest_cap = src_len * 4 + 4;
        unsigned char *dest = (unsigned char *)malloc(dest_cap * sizeof(unsigned char));
        if (!dest) {                       /* allocation failure – clean up */
            free(src);
            return (seq_t)0;
        }
        idx_t dest_len = 0;

        /* Process non‑overlapping pairs */
        for (idx_t i = 0; i + 1 < src_len; i += 2) {
            unsigned char a0 = src[i];
            unsigned char a1 = src[i + 1];
            if (a0 == 0 && a1 == 0) {      /* 00 → 0010 */
                dest[dest_len++] = 0;
                dest[dest_len++] = 0;
                dest[dest_len++] = 1;
                dest[dest_len++] = 0;
            } else if (a0 == 0 && a1 == 1) {/* 01 → 010 */
                dest[dest_len++] = 0;
                dest[dest_len++] = 1;
                dest[dest_len++] = 0;
            } else if (a0 == 1 && a1 == 0) {/* 10 → 000 */
                dest[dest_len++] = 0;
                dest[dest_len++] = 0;
                dest[dest_len++] = 0;
            } /* the pair 11 never occurs in this sequence */
        }

        free(src);
        src = dest;
        src_len = dest_len;
    }

    seq_t result = (seq_t)src[n - 1];   /* zero‑based index */
    free(src);
    return result;
}
