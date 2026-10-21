/*
 * A289011  Fixed point of the mapping 00->0010, 01->001, 10->100, starting
 *          with 00.
 * https://oeis.org/A289011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth bit of the fixed point of the morphism 00→0010,
 *            01→001, 10→100, starting with 00
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; we need the character at position n-1 */
    size_t target = (size_t)n;
    if (target == 0) return (seq_t)0;   /* never used, guard */

    /* Two buffers for successive iterations.  */
    size_t cap = 64;
    uint8_t *cur  = (uint8_t *)malloc(cap);
    uint8_t *next = (uint8_t *)malloc(cap);
    cur[0] = 0;   /* initial word "00" */
    cur[1] = 0;
    size_t len = 2;

    while (len < target) {
        size_t pairs = len / 2;
        size_t new_len = 0;
        /* first pass: compute length of the new word */
        for (size_t i = 0; i < pairs; ++i) {
            uint8_t a = cur[2 * i];
            uint8_t b = cur[2 * i + 1];
            if (a == 0 && b == 0)      new_len += 4;
            else if (a == 0 && b == 1) new_len += 3;
            else if (a == 1 && b == 0) new_len += 3;
            else                        new_len += 2;   /* 11 – not expected */
        }
        if (len % 2) new_len += 1;            /* lone last bit */

        if (new_len > cap) {                  /* enlarge buffer if necessary */
            cap = new_len;
            next = (uint8_t *)realloc(next, cap);
        }

        /* second pass: actually build the new word */
        size_t pos = 0;
        for (size_t i = 0; i < pairs; ++i) {
            uint8_t a = cur[2 * i];
            uint8_t b = cur[2 * i + 1];
            if (a == 0 && b == 0) {
                next[pos++] = 0;
                next[pos++] = 0;
                next[pos++] = 1;
                next[pos++] = 0;
            } else if (a == 0 && b == 1) {
                next[pos++] = 0;
                next[pos++] = 0;
                next[pos++] = 1;
            } else if (a == 1 && b == 0) {
                next[pos++] = 1;
                next[pos++] = 0;
                next[pos++] = 0;
            } else {                           /* 11 – not expected but copied */
                next[pos++] = 1;
                next[pos++] = 1;
            }
        }
        if (len % 2) {                        /* copy the last single bit */
            next[pos++] = cur[len - 1];
        }

        /* swap current and next buffers for next iteration */
        uint8_t *tmp  = cur;
        cur         = next;
        next        = tmp;
        len         = new_len;
    }

    seq_t result = (seq_t)cur[target - 1];
    free(cur);
    free(next);
    return result;
}
