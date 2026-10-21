/*
 * A291241  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S - S^2 + S^3.
 * https://oeis.org/A291241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 4*a(n-4) + a(n-5) +
 *            a(n-6); a(0)=1, a(1)=2, a(2)=3, a(3)=7, a(4)=10, a(5)=22,
 *            a(6)=32
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 86, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *buf = NULL;
    static size_t len = 0;   /* number of computed elements (next free index) */
    static size_t cap = 0;   /* allocated capacity */

    /* convert idx_t to size_t – safe for the sized up to the used range */
    size_t target = (size_t)n;

    /* Expand the buffer while necessary */
    while (len <= target) {
        /* Ensure there is space for the next element */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            while (new_cap <= len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure – return 0 */
            buf = tmp;
            cap = new_cap;
        }

        /* Compute the next term */
        if (len == 0)          buf[len] = (seq_t)1;
        else if (len == 1)     buf[len] = (seq_t)2;
        else if (len == 2)     buf[len] = (seq_t)3;
        else if (len == 3)     buf[len] = (seq_t)7;
        else if (len == 4)     buf[len] = (seq_t)10;
        else if (len == 5)     buf[len] = (seq_t)22;
        else if (len == 6)     buf[len] = (seq_t)32;
        else {
            seq_t a1 = buf[len-1];
            seq_t a2 = buf[len-2];
            seq_t a3 = buf[len-3];
            seq_t a4 = buf[len-4];
            seq_t a5 = buf[len-5];
            seq_t a6 = buf[len-6];
            seq_t temp = a1 + 4*a2 - 3*a3 - 4*a4 + a5 + a6;
            buf[len] = temp;
        }
        ++len;
    }

    return buf[target];
}
