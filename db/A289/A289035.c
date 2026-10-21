/*
 * A289035  Fixed point of the mapping 00->0010, 01->010, 10->010, starting
 *          with 00.
 * https://oeis.org/A289035
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

/* Computes a(n): nth term (1‑indexed) of the fixed point word obtained by
   iteratively applying the mapping
      00 → 0010
      01 → 010
      10 → 010
   to the starting word "00".  The word is built until it contains at least n
   bits; the required bit is then returned. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset : a(1) is the first term */

    /* allocate two buffers large enough for the final string.
       At each iteration the length grows by at most a factor of two,
       so we reserve 2*n+10 bytes.  */
    size_t target = (size_t)n;
    size_t bufsize = 2 * target + 10;
    unsigned char *buf1 = (unsigned char *)malloc(bufsize);
    unsigned char *buf2 = (unsigned char *)malloc(bufsize);
    if (!buf1 || !buf2)                  /* allocation failure – abort */
        return (seq_t)0;

    /* initial word "00" */
    size_t len = 2;
    buf1[0] = 0;  buf1[1] = 0;

    while (len < target) {
        unsigned char *src = buf1;
        unsigned char *dst = buf2;
        size_t pos = 0, newlen = 0;

        /* process 2‑bit blocks */
        while (pos + 1 < len) {
            unsigned char a0 = src[pos];
            unsigned char a1 = src[pos + 1];
            if (a0 == 0 && a1 == 0) {          /* 00 → 0010 */
                dst[newlen++] = 0;
                dst[newlen++] = 0;
                dst[newlen++] = 1;
                dst[newlen++] = 0;
            } else {                            /* 01 or 10 → 010 */
                dst[newlen++] = 0;
                dst[newlen++] = 1;
                dst[newlen++] = 0;
            }
            pos += 2;
        }
        /* if the word had odd length, copy the final unpaired bit */
        if (pos < len)
            dst[newlen++] = src[pos];

        /* swap buffers and update length */
        unsigned char *tmp = buf1;
        buf1 = buf2;
        buf2 = tmp;
        len = newlen;
    }

    seq_t result = (seq_t)buf1[target - 1];
    free(buf1);
    free(buf2);
    return result;
}
