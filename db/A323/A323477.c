/*
 * A323477  Successive generations of A306211, in compressed notation.
 * https://oeis.org/A323477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal number formed by concatenating the terms of the
 *            n-th generation G(n) of A306211, where G(1) = [1] and G(k+1) =
 *            G(k) followed by the run lengths of G(k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    size_t len = 1, cap = 16;
    unsigned char *s = (unsigned char *)malloc(cap);
    s[0] = 1;
    for (idx_t g = 1; g < n; ++g) {
        /* number of runs is at most len, so the new length is at most 2*len */
        if (cap < 2 * len) {
            while (cap < 2 * len) cap *= 2;
            s = (unsigned char *)realloc(s, cap);
        }
        size_t out = len;
        size_t run = 1;
        for (size_t i = 0; i + 1 < len; ++i) {
            if (s[i] == s[i + 1]) {
                ++run;
            } else {
                s[out++] = (unsigned char)run;
                run = 1;
            }
        }
        s[out++] = (unsigned char)run;
        len = out;
    }
    seq_t v = 0;
    for (size_t i = 0; i < len; ++i) {
        v = v * 10 + (seq_t)s[i];
    }
    free(s);
    return v;
}
