/*
 * A287931  Fixed point of the mapping 00->0010, 1->10, starting with 00.
 * https://oeis.org/A287931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth bit (1‑indexed) of the fixed point of the
 *            morphism 00 → 0010, 1 → 10, starting with the word "00"
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *buf = NULL;
    static idx_t len   = 0;
    static idx_t cap   = 0;

    /* Initial seed: the starting word "00" */
    if (len == 0) {
        cap   = 4;                      /* small initial capacity */
        buf   = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!buf) return 0;              /* allocation failure */
        buf[0] = 0;
        buf[1] = 0;
        len   = 2;
    }

    if (n == 0) return 0;                /* undefined index, return 0 */

    /* Generate the word until its length reaches or exceeds n */
    while (len < n) {
        idx_t i = 0;
        idx_t newlen = 0;
        idx_t needed_cap = len * 2 + 10; /* worst case size estimate */
        seq_t *newbuf = (seq_t*)malloc(needed_cap * sizeof(seq_t));
        if (!newbuf) return 0;           /* allocation failure */

        while (i < len) {
            if (buf[i] == 1) {           /* 1 → 10 */
                newbuf[newlen++] = 1;
                newbuf[newlen++] = 0;
                i++;
            } else {                     /* buf[i] == 0 */
                if (i + 1 < len && buf[i + 1] == 0) {   /* 00 → 0010 */
                    newbuf[newlen++] = 0;
                    newbuf[newlen++] = 0;
                    newbuf[newlen++] = 1;
                    newbuf[newlen++] = 0;
                    i += 2;
                } else {                     /* single 0 → 0 */
                    newbuf[newlen++] = 0;
                    i++;
                }
            }
        }

        free(buf);
        buf = newbuf;
        len = newlen;
        cap = needed_cap;
    }

    return buf[n - 1];                     /* 1‑indexed access */
}
