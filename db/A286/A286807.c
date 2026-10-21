/*
 * A286807  Fixed point of the mapping 00->001, 1->101, starting with 00.
 * https://oeis.org/A286807
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

/* Fixed point of mapping 00->001, 1->101, starting with 00 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of the current word bits (0 or 1) */
    static unsigned char *bits = NULL;
    static size_t len = 0;

    /* initialise with the starting word "00" */
    if (len == 0) {
        bits = (unsigned char*)malloc(2 * sizeof(unsigned char));
        bits[0] = 0;
        bits[1] = 0;
        len = 2;
    }

    /* grow until we have at least n bits */
    while (len < (size_t)n) {
        size_t i = 0;
        size_t new_len = 0;

        /* first pass: compute length of the next word */
        while (i < len) {
            if (i + 1 < len && bits[i] == 0 && bits[i + 1] == 0) {
                new_len += 3;   /* "00" -> "001" */
                i += 2;
            } else if (bits[i] == 1) {
                new_len += 3;   /* "1" -> "101" */
                i += 1;
            } else {
                new_len += 1;   /* single "0" stays "0" */
                i += 1;
            }
        }

        /* second pass: construct the new word */
        unsigned char *new_bits = (unsigned char*)malloc(new_len * sizeof(unsigned char));
        i = 0;
        size_t j = 0;
        while (i < len) {
            if (i + 1 < len && bits[i] == 0 && bits[i + 1] == 0) {
                new_bits[j++] = 0;
                new_bits[j++] = 0;
                new_bits[j++] = 1;
                i += 2;
            } else if (bits[i] == 1) {
                new_bits[j++] = 1;
                new_bits[j++] = 0;
                new_bits[j++] = 1;
                i += 1;
            } else {
                new_bits[j++] = 0;
                i += 1;
            }
        }

        free(bits);
        bits = new_bits;
        len = new_len;
    }

    return (seq_t)bits[(size_t)n - 1];
}
