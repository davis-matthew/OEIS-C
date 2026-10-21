/*
 * A275661  a(1)=1. At any step consider the stream of 0's and 1's as two
 *          base-2 numbers: the first reading from left to right and the
 *          second from right to left. Sum the two numbers in base 2 and add
 *          them to the end of the sequence. Repeat.
 * https://oeis.org/A275661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275661: start with the bit string S = "1".
   For each step, interpret S left‑to‑right as binary number A
   and right‑to‑left as binary number B.  Append the binary
   representation of A+B to S.  a(n) is the n‑th bit of the
   resulting infinite string. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state */
    static uint8_t *bits = NULL; /* accumulated bit string */
    static size_t len   = 0;      /* current length of bits */
    static size_t cap   = 0;      /* capacity of bits array */

    /* initialise on first call */
    if (bits == NULL) {
        bits = (uint8_t *)malloc(sizeof(uint8_t));
        bits[0] = 1;    /* first term a(1) = 1 */
        len = 1;
        cap = 1;
    }

    /* grow until we have at least n bits */
    if ((size_t)n > len) {
        while ((size_t)n > len) {
            size_t cur_len = len;

            /* compute sum of current string and its reverse */
            size_t temp_cap = cur_len + 1;
            uint8_t *temp = (uint8_t *)malloc(temp_cap * sizeof(uint8_t));
            size_t tpos = 0;
            int carry = 0;

            for (size_t j = 0; j < cur_len; ++j) {
                int bit1 = bits[cur_len - 1 - j]; /* LSB‑to‑MSB for A */
                int bit2 = bits[j];               /* reversed for B */
                int s = bit1 + bit2 + carry;
                temp[tpos++] = s & 1;         /* LSB of current sum digit */
                carry = s >> 1;               /* carry to next digit */
            }
            if (carry) temp[tpos++] = carry;   /* final carry */

            /* append the sum bits (MSB→LSB) to the main sequence */
            size_t new_len = len + tpos;
            if (new_len > cap) {
                size_t new_cap = cap ? (cap << 1) : 16;
                while (new_cap < new_len) new_cap <<= 1;
                bits = (uint8_t *)realloc(bits, new_cap * sizeof(uint8_t));
                cap = new_cap;
            }
            for (size_t i = tpos; i-- > 0; ) {
                bits[len++] = temp[i];     /* MSB first */
            }
            free(temp);
        }
    }

    return (seq_t)bits[n - 1];
}
