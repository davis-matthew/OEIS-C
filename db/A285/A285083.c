/*
 * A285083  1-limiting word of the morphism 0->10, 1-> 011.
 * https://oeis.org/A285083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth term of the 1‑limiting word of the morphism 0 → 10, 1
 *            → 011. The word is obtained by iterating the morphism a 2‑step
 *            cycle (odd index) starting from “10”; the nth symbol stabilises
 *            after O(log n) morphisms
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static buffer containing the odd‑iteration word up to current length */
    static uint8_t *buf = NULL;
    static size_t buf_len = 0;
    /* initialise with the first odd iteration (length 2) */
    if (buf_len == 0) {
        buf_len = 2;
        buf = (uint8_t*)malloc(buf_len);
        buf[0] = 1;
        buf[1] = 0;
    }
    /* enlarge buffer until it contains at least n symbols */
    while ((size_t)n > buf_len) {
        size_t old_len = buf_len;
        size_t new_len = 0;
        /* compute length after two morphism steps:
           0 → 01110  (5 bits), 1 → 10011011 (8 bits) */
        for (size_t i = 0; i < old_len; ++i)
            new_len += (buf[i] == 0 ? 5 : 8);
        uint8_t *new_buf = (uint8_t*)malloc(new_len);
        size_t pos = 0;
        for (size_t i = 0; i < old_len; ++i) {
            if (buf[i] == 0) {
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 1;
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
            } else {
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 1;
            }
        }
        free(buf);
        buf = new_buf;
        buf_len = new_len;
    }
    /* n is 1‑based; convert to 0‑based index */
    return (seq_t)buf[(size_t)n - 1];
}
