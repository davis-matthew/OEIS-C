/*
 * A284394  {101->2}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A284394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   generate the infinite Fibonacci word via morphism 0→01, 1→0,
 *            then produce the transformed word by replacing each
 *            non‑overlapping 101 by 2; return a(n) (1‑indexed)
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
    if (n == 0) return (seq_t)0;                 /* safety; offset is 1 */

    /* a safe upper bound for how many original bits are needed. */
    size_t target = (size_t)n * 3ULL;
    char *buf = (char *)malloc(target + 1);
    if (!buf) return (seq_t)0;                   /* out of memory guard   */

    size_t len = 1;
    buf[0] = '0';

    /* Build the Fibonacci word until it has at least 'target' bits. */
    while (len < target) {
        size_t newlen = 0;
        for (size_t i = 0; i < len; ++i)
            newlen += (buf[i] == '0') ? 2 : 1;

        char *newbuf = (char *)malloc(newlen + 1);
        if (!newbuf) { free(buf); return (seq_t)0; }

        size_t pos = 0;
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] == '0') {
                newbuf[pos++] = '0';
                newbuf[pos++] = '1';
            } else {
                newbuf[pos++] = '0';
            }
        }
        newbuf[newlen] = '\0';

        free(buf);
        buf = newbuf;
        len = newlen;
    }

    /* Scan the word, transforming 101→2, and return the nth output symbol. */
    size_t outcnt = 0;
    size_t i = 0;
    seq_t value = 0;
    while (outcnt < (size_t)n) {
        if (i + 2 < len && buf[i] == '1' && buf[i + 1] == '0' && buf[i + 2] == '1') {
            value = (seq_t)2;
            i += 3;
        } else {
            value = (seq_t)(buf[i] - '0');  /* 0 or 1 */
            i += 1;
        }
        ++outcnt;
    }

    free(buf);
    return value;
}
