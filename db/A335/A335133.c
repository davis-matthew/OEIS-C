/*
 * A335133  Binary interpretation of the left diagonal of the EQ-triangle with
 *          first row generated from the binary expansion of n, with most
 *          significant bit given by first row.
 * https://oeis.org/A335133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) is the binary value obtained from the left diagonal of the
 *            EQ‑triangle built from the binary expansion of n. The first row
 *            is the binary digits of n (most significant first). Each
 *            subsequent row contains the equality (1 if the two digits above
 *            are equal, 0 otherwise). Taking the first elemen...
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /*  for size_t  */

#define MAX_BITS 64   /*  enough for all 64‑bit idx_t values  */

seq_t a(idx_t n)
{
    /*  Determine the number of bits of n (at least 1).  */
    size_t L = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        L = 1;
    } else {
        while (tmp) {
            ++L;
            tmp >>= 1;
        }
    }

    /*  Store the binary digits of n (MSB first).  */
    unsigned char bits[MAX_BITS];
    for (size_t i = 0; i < L; ++i) {
        bits[i] = (unsigned char)((n >> (L - 1 - i)) & 1);
    }

    /*  Buffers for the current and next rows of the triangle.  */
    unsigned char buf0[MAX_BITS];
    unsigned char buf1[MAX_BITS];
    unsigned char *cur  = buf0;   /*  pointer to current row  */
    unsigned char *next = buf1;   /*  pointer to next row     */

    for (size_t i = 0; i < L; ++i) cur[i] = bits[i];
    size_t cur_len = L;

    seq_t diag = 0;

    /*  Walk down the triangle, collecting the left diagonal bits.  */
    while (cur_len > 0) {
        diag = (diag << 1) | (seq_t)cur[0];
        if (cur_len == 1)   /*  last row reached  */
            break;
        for (size_t i = 0; i < cur_len - 1; ++i) {
            next[i] = (cur[i] == cur[i + 1]) ? 1 : 0;
        }
        /*  Swap the buffers for the next iteration.  */
        unsigned char *tmp = cur;
        cur = next;
        next = tmp;
        --cur_len;
    }

    return diag;
}
