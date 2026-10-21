/*
 * A290742  Orbit of word "1" under the 3-shift tag system over the alphabet
 *          {1,2} defined in the Comments.
 * https://oeis.org/A290742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Orbit of word "1" under the 3‑shift tag system over {1,2} (simulation). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    size_t cap = 1 + 4 * ((size_t)n - 1) + 4;   /* space for final length + safety */
    uint8_t *buf = (uint8_t *)malloc(cap);
    if (!buf) return 0;                        /* allocation failure */

    size_t head = 0;
    size_t len  = 1;
    buf[0] = 1;                                 /* initial word "1" */

    static const uint8_t add1[4] = {2, 1, 1, 2};           /* for first symbol 1 */
    static const uint8_t add2[7] = {1, 2, 2, 1, 2, 2, 2};   /* for first symbol 2 */

    for (idx_t step = 1; step < n; ++step) {
        uint8_t first = buf[head];
        const uint8_t *add;
        size_t addlen;

        if (first == 1) {
            add = add1;
            addlen = 4;
        } else {
            add = add2;
            addlen = 7;
        }

        /* append replacement */
        for (size_t k = 0; k < addlen; ++k) {
            buf[(head + len) % cap] = add[k];
            ++len;
        }

        /* delete first three symbols */
        head = (head + 3) % cap;
        len -= 3;
    }

    /* convert word to integer */
    seq_t value = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = buf[(head + i) % cap];
        value = value * 10 + d;
    }

    free(buf);
    return value;
}
