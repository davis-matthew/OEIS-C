/*
 * A285080  0-limiting word of the morphism 0->10, 1-> 011.
 * https://oeis.org/A285080
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

/* Fixed point morphism 0→10, 1→011.
   a(n) is the n‑th bit (0‑based index n‑1) of the even‑iteration limit word
   (the 0‑limiting word). */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *buf = NULL;
static size_t buf_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* safety for n=0 */

    /* initialise with the single symbol 0 (w0) */
    if (!buf) {
        buf = (unsigned char*)malloc(1);
        if (!buf) abort();
        buf[0] = 0;
        buf_len = 1;
    }

    /* grow the word until it contains at least n symbols
       only even iterations are kept (0‑word) */
    while (buf_len < n) {
        /* first expansion: f applied to buf → tmp1 */
        size_t len1 = 0;
        for (size_t i = 0; i < buf_len; ++i)
            len1 += (buf[i] == 0) ? 2 : 3;

        unsigned char *tmp1 = (unsigned char*)malloc(len1);
        if (!tmp1) abort();
        size_t pos = 0;
        for (size_t i = 0; i < buf_len; ++i) {
            if (buf[i] == 0) {        /* 0 → 1 0 */
                tmp1[pos++] = 1;
                tmp1[pos++] = 0;
            } else {                  /* 1 → 0 1 1 */
                tmp1[pos++] = 0;
                tmp1[pos++] = 1;
                tmp1[pos++] = 1;
            }
        }

        /* second expansion: f applied to tmp1 → tmp2 */
        size_t len2 = 0;
        for (size_t i = 0; i < len1; ++i)
            len2 += (tmp1[i] == 0) ? 2 : 3;

        unsigned char *tmp2 = (unsigned char*)malloc(len2);
        if (!tmp2) abort();
        pos = 0;
        for (size_t i = 0; i < len1; ++i) {
            if (tmp1[i] == 0) {        /* 0 → 1 0 */
                tmp2[pos++] = 1;
                tmp2[pos++] = 0;
            } else {                  /* 1 → 0 1 1 */
                tmp2[pos++] = 0;
                tmp2[pos++] = 1;
                tmp2[pos++] = 1;
            }
        }

        free(buf);
        free(tmp1);
        buf = tmp2;
        buf_len = len2;
    }

    return (seq_t)(unsigned long long)buf[n - 1];
}
