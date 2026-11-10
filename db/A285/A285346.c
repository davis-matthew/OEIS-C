/*
 * A285346  Positions of 0 in A285345; complement of A285347.
 * https://oeis.org/A285346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285345, the fixed point of the
 *            morphism 0 -> 10, 1 -> 1100. The code builds the word by
 *            self-expansion and scans it for zeros
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9752 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    size_t L = (size_t)n * 2 + 16;
    uint8_t *w = (uint8_t *)malloc(L);
    if (!w) return 0;
    size_t len = 0, i = 1;
    /* The fixed point begins with the image of 1, which is 1100. */
    w[len++] = 1; w[len++] = 1; w[len++] = 0; w[len++] = 0;
    while (len < L && i < len) {
        if (w[i] == 0) {
            w[len++] = 1;
            if (len < L) w[len++] = 0;
        } else {
            w[len++] = 1;
            if (len < L) w[len++] = 1;
            if (len < L) w[len++] = 0;
            if (len < L) w[len++] = 0;
        }
        i++;
    }
    idx_t cnt = 0;
    size_t pos = 0;
    for (size_t k = 0; k < len; k++) {
        if (w[k] == 0) {
            cnt++;
            if (cnt == n) { pos = k + 1; break; }
        }
    }
    free(w);
    return (seq_t)pos;
}
