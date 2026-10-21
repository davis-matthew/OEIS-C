/*
 * A288377  Fixed point of the mapping 00->0001, 1->10, starting with 00.
 * https://oeis.org/A288377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol of the fixed point of the rewriting
 *            system: Scan left to right: replace each "00" with "0001", each
 *            single '1' with "10", and a lone '0' stays '0'
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of the growing prefix */
    static seq_t *buf  = NULL;
    static idx_t buflen = 0;

    if (n == 0) return (seq_t)0;                 /* safety, n>=1 in usage */

    if (buf == NULL) {
        /* start with "00" */
        buf = (seq_t*)malloc(4 * sizeof(seq_t));
        buf[0] = (seq_t)0;
        buf[1] = (seq_t)0;
        buflen = 2;
    }

    /* extend until we have at least n symbols */
    while (buflen < n) {
        idx_t new_cap = buflen * 2 + 4;                 /* worst case */
        seq_t *newbuf = (seq_t*)malloc(new_cap * sizeof(seq_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < buflen; ) {
            if (buf[i] == (seq_t)0 && i + 1 < buflen && buf[i + 1] == (seq_t)0) {
                /* "00" -> "0001" */
                newbuf[pos++] = (seq_t)0;
                newbuf[pos++] = (seq_t)0;
                newbuf[pos++] = (seq_t)0;
                newbuf[pos++] = (seq_t)1;
                i += 2;
            } else if (buf[i] == (seq_t)1) {
                /* "1" -> "10" */
                newbuf[pos++] = (seq_t)1;
                newbuf[pos++] = (seq_t)0;
                i += 1;
            } else {
                /* single "0" stays "0" */
                newbuf[pos++] = (seq_t)0;
                i += 1;
            }
        }
        free(buf);
        buf = newbuf;
        buflen = pos;
    }

    return buf[n - 1];
}
