/*
 * A284683  Fixed point of the morphism 0 -> 01, 1 -> 0000.
 * https://oeis.org/A284683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol of the fixed point of the morphism * 0 ->
 *            01, 1 -> 0000. The routine expands the word dynamically * until
 *            it has at least n symbols and then returns the n‑th value
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, return 0 */

    static unsigned char *buf = NULL; /* raw 0/1 storage */
    static size_t buf_len = 0;
    static size_t buf_cap = 0;

    /* convert to 0‑based index */
    size_t target = (size_t)(n - 1);

    /* grow the sequence until it covers the requested position */
    if (buf_len <= target) {
        /* initial entry if not already defined */
        if (buf_cap == 0) {
            buf_cap = 1;
            buf = (unsigned char *)malloc(buf_cap);
            buf[0] = 0;            /* start with symbol 0 */
            buf_len = 1;
        }

        /* iteratively apply the morphism until we surpass target */
        while (buf_len <= target) {
            /* determine new length */
            size_t new_len = 0;
            for (size_t i = 0; i < buf_len; ++i)
                new_len += (buf[i] == 0) ? 2 : 4;

            /* allocate new buffer */
            unsigned char *new_buf = (unsigned char *)malloc(new_len);
            size_t pos = 0;
            for (size_t i = 0; i < buf_len; ++i) {
                if (buf[i] == 0) {
                    new_buf[pos++] = 0;
                    new_buf[pos++] = 1;
                } else {
                    new_buf[pos++] = 0;
                    new_buf[pos++] = 0;
                    new_buf[pos++] = 0;
                    new_buf[pos++] = 0;
                }
            }
            free(buf);
            buf = new_buf;
            buf_len = new_len;
            buf_cap = new_len;
        }
    }

    return (seq_t)buf[target];
}
