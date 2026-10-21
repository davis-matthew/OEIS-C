/*
 * A288372  Fixed point of the mapping 00->0001, 1->01, starting with 00.
 * https://oeis.org/A288372
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

/* Computation: fixed point of mapping 00->0001, 1->01 starting from 00 */
/* The sequence consists of the bits of the resulting infinite binary word. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static buffers to memoize the fixed‑point prefix */
    static unsigned char *buf = NULL;
    static size_t len = 0;

    if (n == 0)                 /* not used in OEIS, but safe */
        return (seq_t)0;

    /* initialise with the first word "00" */
    if (buf == NULL) {
        buf = (unsigned char *)malloc(2 * sizeof(unsigned char));
        buf[0] = 0;
        buf[1] = 0;
        len = 2;
    }

    /* grow the prefix until it contains at least n bits */
    while (len < (size_t)n) {
        /* first pass: compute length of the next word */
        size_t i = 0;
        size_t next_len = 0;
        while (i < len) {
            if (buf[i] == 0 && i + 1 < len && buf[i + 1] == 0) {
                next_len += 4;          /* 00 -> 0001 */
                i += 2;
            } else {
                next_len += (buf[i] == 1) ? 2 : 1;   /* 1 -> 01 or single 0 -> 0 */
                i += 1;
            }
        }

        /* second pass: build the next word */
        unsigned char *newbuf = (unsigned char *)malloc(next_len * sizeof(unsigned char));
        i = 0;
        size_t p = 0;
        while (i < len) {
            if (buf[i] == 0 && i + 1 < len && buf[i + 1] == 0) {
                newbuf[p++] = 0;
                newbuf[p++] = 0;
                newbuf[p++] = 0;
                newbuf[p++] = 1;
                i += 2;
            } else {
                if (buf[i] == 1) {
                    newbuf[p++] = 0;
                    newbuf[p++] = 1;
                } else {
                    newbuf[p++] = 0;
                }
                i += 1;
            }
        }

        free(buf);
        buf = newbuf;
        len = next_len;
    }

    return (seq_t)buf[n - 1];
}
