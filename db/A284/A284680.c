/*
 * A284680  Fixed point of the morphism 0->1, 1->1000.
 * https://oeis.org/A284680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Fixed point of the morphism 0 -> 1, 1 -> 1000
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *buf = NULL;   /* current word */
static size_t len  = 0;     /* length of current word */

seq_t a(idx_t n)
{
    /* initialise with the first word 1 */
    if (!buf) {
        buf = (seq_t*)malloc(sizeof(seq_t));
        buf[0] = 1;
        len  = 1;
    }

    /* expand until the word is long enough */
    while ((idx_t)len < n) {
        size_t new_len = 0;
        for (size_t i = 0; i < len; ++i) {
            new_len += (buf[i] == 0) ? 1 : 4;
        }

        seq_t *new_buf = (seq_t*)malloc(new_len * sizeof(seq_t));
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] == 0) {
                new_buf[pos++] = 1;            /* 0 → 1 */
            } else {
                new_buf[pos++] = 1;            /* 1 → 1000 */
                new_buf[pos++] = 0;
                new_buf[pos++] = 0;
                new_buf[pos++] = 0;
            }
        }

        free(buf);
        buf = new_buf;
        len = new_len;
    }

    return buf[n - 1];
}
