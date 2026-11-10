/*
 * A248130  Limiting sequence of digits preceding the 0's in A248128.
 * https://oeis.org/A248130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit at position m-n of the digit stream of A248128
 *            (stream = 5,0, then digits of 3*d for each digit d read in
 *            order), where m is the position of a late 0 whose preceding
 *            zero-free gap exceeds n; computed by generating the stream and
 *            searching
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *A248130_dg = NULL;
static size_t A248130_len = 0, A248130_cap = 0, A248130_rd = 0;

static void A248130_ensure(size_t want)
{
    if (A248130_dg == NULL) {
        A248130_cap = 1024;
        A248130_dg = (uint8_t *)malloc(A248130_cap);
        A248130_dg[0] = 5;
        A248130_dg[1] = 0;
        A248130_len = 2;
        A248130_rd = 0;
    }
    if (A248130_cap < want + 4) {
        while (A248130_cap < want + 4) A248130_cap *= 2;
        A248130_dg = (uint8_t *)realloc(A248130_dg, A248130_cap);
    }
    while (A248130_len < want) {
        unsigned v = 3u * A248130_dg[A248130_rd++];
        if (v >= 10) {
            A248130_dg[A248130_len++] = (uint8_t)(v / 10);
            A248130_dg[A248130_len++] = (uint8_t)(v % 10);
        } else {
            A248130_dg[A248130_len++] = (uint8_t)v;
        }
    }
}

seq_t a(idx_t n)
{
    size_t nn = (size_t)n;
    size_t want = 4096;
    if (want < nn * 16 + 64) want = nn * 16 + 64;
    for (;;) {
        A248130_ensure(want);
        size_t m = A248130_len;
        while (m > 0 && A248130_dg[m - 1] != 0) m--;
        if (m > 0) {
            m--; /* last zero index */
            size_t p = m;
            while (p > 0 && A248130_dg[p - 1] != 0) p--;
            if (p > 0) {
                p--; /* previous zero index */
                if (m - p > nn)
                    return (seq_t)A248130_dg[m - nn];
            }
        }
        want *= 2;
    }
}
