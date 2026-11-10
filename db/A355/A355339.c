/*
 * A355339  a(n) is the number of central even terms in the n-th row of
 *          triangle A355320; a(0) = 0.
 * https://oeis.org/A355339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the maximal run of consecutive even terms
 *            centred on the middle term of row n of A355320 (0 if the middle
 *            term is odd); computed on parities via T(n,i) = T(n-1,i-4) +
 *            T(n-1,i) + T(n-2,i-5) + T(n-2,i-3) mod 2, T(0,0) = 1
 * Range:     u8 n <= 382, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 93 terms through n = 8144 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    size_t w = 4 * N + 1;
    unsigned char *p2 = (unsigned char *)calloc(w + 8, 1);
    unsigned char *p1 = (unsigned char *)calloc(w + 8, 1);
    unsigned char *cur = (unsigned char *)calloc(w + 8, 1);
    p1[0] = 1; /* row 0 */
    for (size_t r = 1; r <= N; ++r) {
        size_t sz = 4 * r + 1;
        size_t s1 = 4 * (r - 1) + 1;
        size_t s2 = (r >= 2) ? 4 * (r - 2) + 1 : 0;
        for (size_t i = 0; i < sz; ++i) {
            unsigned char s = 0;
            if (i >= 4 && i - 4 < s1) s ^= p1[i - 4];
            if (i < s1) s ^= p1[i];
            if (i >= 5 && i - 5 < s2) s ^= p2[i - 5];
            if (i >= 3 && i - 3 < s2) s ^= p2[i - 3];
            cur[i] = s;
        }
        unsigned char *t = p2;
        p2 = p1;
        p1 = cur;
        cur = t;
        memset(cur, 0, w + 8);
    }
    size_t c = 2 * N;
    size_t count = 0;
    if (p1[c] == 0) {
        count = 1;
        size_t j = c;
        while (j > 0 && p1[j - 1] == 0) { ++count; --j; }
        j = c;
        while (j + 1 < 4 * N + 1 && p1[j + 1] == 0) { ++count; ++j; }
    }
    free(p2);
    free(p1);
    free(cur);
    return (seq_t)count;
}
