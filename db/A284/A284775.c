/*
 * A284775  Fixed point of the morphism 0 -> 01, 1 -> 0011.
 * https://oeis.org/A284775
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

/* Computed by iterating the morphism 0 → 01, 1 → 0011 until a
 * word of length at least n is obtained.  The sequence contains
 * only the values 0 and 1. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t buf_len = 0;

    /* initialise the fixed point seed */
    if (buf_len == 0) {
        buf_len = 1;
        buf = (seq_t *)malloc(buf_len * sizeof(seq_t));
        buf[0] = (seq_t)0;
    }

    /* grow the word until it is long enough for the requested n */
    while (buf_len < (size_t)n) {
        size_t old_len = buf_len;
        seq_t *newbuf = (seq_t *)malloc(4 * old_len * sizeof(seq_t));
        size_t pos = 0;

        for (size_t i = 0; i < old_len; ++i) {
            if (buf[i] == (seq_t)0) {          /* 0 → 01 */
                newbuf[pos++] = (seq_t)0;
                newbuf[pos++] = (seq_t)1;
            } else {                           /* 1 → 0011 */
                newbuf[pos++] = (seq_t)0;
                newbuf[pos++] = (seq_t)0;
                newbuf[pos++] = (seq_t)1;
                newbuf[pos++] = (seq_t)1;
            }
        }

        free(buf);
        buf     = newbuf;
        buf_len = pos;   /* actual length after expansion */
    }

    /* n is 1‑based; buffer indices are 0‑based */
    size_t index = (size_t)(n - 1);
    return buf[index];
}
