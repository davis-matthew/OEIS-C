/*
 * A395088  Fixed point of the morphism 0->011, 1->0.
 * https://oeis.org/A395088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10922, u16 n <= 10922, u32 n <= 10922, u64 n <= 10922,
 *            u128 n <= 10922, big any n
 * Validated: standard, pass, 103 terms through n = 9881 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth bit of the fixed point of the morphism
 * 0 → 011, 1 → 0.  The function builds the word by repeatedly
 * applying the morphism until the required index is present.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static state that keeps the generated word across calls */
    static seq_t *buf = NULL;      /* buffer of bits */
    static idx_t buf_len = 0;      /* current length of buffer      */
    static idx_t buf_cap = 0;      /* current capacity of buffer    */

    /* initialise the buffer with the seed symbol '0' */
    if (buf == NULL) {
        buf_cap = 8;
        buf = mem_new(seq_t, buf_cap);
        if (buf == NULL) return 0;          /* out of memory → safe default */
        buf[0] = 0;
        buf_len = 1;
    }

    /* grow the buffer until it contains the desired index */
    while (buf_len <= n) {
        idx_t old_len = buf_len;
        idx_t new_len = 0;

        /* compute the length of the next expansion */
        for (idx_t i = 0; i < old_len; ++i) {
            new_len += (buf[i] == 0 ? 3 : 1);
        }

        /* resize buffer if needed */
        if (new_len > buf_cap) {
            size_t new_cap = buf_cap ? buf_cap * 2 : 8;
            while (new_cap < new_len) new_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, buf, new_cap);
            if (tmp == NULL) return 0;       /* out of memory → safe default */
            buf = tmp;
            buf_cap = new_cap;
        }

        /* perform the actual expansion */
        idx_t pos = 0;
        for (idx_t i = 0; i < old_len; ++i) {
            if (buf[i] == 0) {
                buf[pos++] = 0;
                buf[pos++] = 1;
                buf[pos++] = 1;
            } else {
                buf[pos++] = 0;
            }
        }
        buf_len = new_len;
    }

    return buf[n];
}
