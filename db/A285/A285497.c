/*
 * A285497  Positions of 1 in A285495; complement of A285496.
 * https://oeis.org/A285497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285495, the fixed point of
 *            0->00100010, 1->1111001011 (the morphism 0->11, 1->0010 applied
 *            twice); computed by expanding the word and scanning
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9525 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static const char *E[2] = { "00100010", "1111001011" };
    static const size_t EL[2] = { 8, 10 };
    size_t L = (size_t)n * 4 + 32;
    for (;;) {
        unsigned char *cur = (unsigned char *)malloc(L);
        unsigned char *nxt = (unsigned char *)malloc(L);
        size_t len = 1;
        cur[0] = 0;
        while (len < L) {
            size_t pos = 0;
            for (size_t i = 0; i < len && pos < L; ++i) {
                int c = cur[i];
                for (size_t k = 0; k < EL[c] && pos < L; ++k)
                    nxt[pos++] = (unsigned char)(E[c][k] - '0');
            }
            unsigned char *t = cur; cur = nxt; nxt = t;
            len = pos;
        }
        size_t count = 0;
        size_t found = 0;
        for (size_t i = 0; i < len; ++i) {
            if (cur[i]) {
                ++count;
                if (count == (size_t)n) { found = i + 1; break; }
            }
        }
        free(cur);
        free(nxt);
        if (found) return (seq_t)found;
        L *= 2;
    }
}
