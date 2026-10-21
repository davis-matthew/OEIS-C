/*
 * A260528  Concatenate the positions of digits 9, 8,..., 0 in the decimal
 *          representation of n, using 1 for the leftmost digit etc., skip
 *          digits which don't occur.
 * https://oeis.org/A260528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Concatenate the positions of each digit 9,8,...,0 in the *
 *            decimal representation of n (1‑based from the left). For
 *            example, * a(1103) = 4123
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External helper functions provided by the build system. */
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Compute a(n) as defined in OEIS A260528. */
seq_t a(idx_t n)
{
    /* First determine the number of decimal digits of n.  Treat n==0
     * specially: its decimal representation is "0". */
    idx_t t = n;
    size_t dcnt = 0;
    if (t == 0) {
        dcnt = 1;
    } else {
        while (t > 0) {
            t /= 10;
            ++dcnt;
        }
    }

    /* Buffer to hold the decimal digits of n from left to right. */
    unsigned char *digbuf = (unsigned char *)malloc(dcnt);
    if (!digbuf) return 0;  /* OOM: return 0 as a fallback. */

    /* Fill digbuf.  We first compute the least significant digits
     * and then reverse the order. */
    t = n;
    if (t == 0) {
        digbuf[0] = 0;
    } else {
        size_t pos = dcnt;
        while (t > 0) {
            --pos;
            digbuf[pos] = (unsigned char)(t % 10);
            t /= 10;
        }
    }

    /* Allocate a contiguous block for all position lists.
     * posbuf[d] will point to the buffer for digit d.
     */
    idx_t *posbuf = (idx_t *)malloc(10 * dcnt * sizeof(idx_t));
    if (!posbuf) {
        free(digbuf);
        return 0;
    }
    idx_t *poslist[10];
    size_t lens[10];
    for (int d = 0; d < 10; ++d) {
        poslist[d] = posbuf + d * dcnt;
        lens[d] = 0;
    }

    /* Record positions of each digit.  Positions are 1‑based from the left. */
    idx_t pos = 1;
    for (size_t i = 0; i < dcnt; ++i) {
        unsigned char cur = digbuf[i];
        poslist[cur][lens[cur]++] = pos;
        ++pos;
    }

    /* Build the concatenated result. */
    seq_t result = 0;
    for (int d = 9; d >= 0; --d) {
        for (size_t i = 0; i < lens[d]; ++i) {
            seq_t pseq = (seq_t)poslist[d][i];
            int plen = digit_len(pseq);           /* number of decimal digits of pseq */
            result = result * pow10((idx_t)plen) + pseq;
        }
    }

    free(digbuf);
    free(posbuf);
    return result;
}
