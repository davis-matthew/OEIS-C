/*
 * A289026  Positions of 0 in A289025; complement of A289027.
 * https://oeis.org/A289026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A289025, the fixed point of
 *            00->0010, 01->010, 10->001 starting with 00 (computed by
 *            iterating the substitution and listing the zero positions)
 * Range:     u8 n <= 181, u16 n <= 24970, u32 n <= 24970, u64 n <= 24970,
 *            u128 n <= 24970, big any n
 * Validated: standard, pass, 82 terms through n = 9998 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static unsigned char *A289026_build_bits(size_t minlen, size_t *outlen)
{
    size_t cl = 2;
    unsigned char *cur = (unsigned char *)malloc(2);
    cur[0] = 0; cur[1] = 0;
    while (cl < minlen) {
        unsigned char *nx = (unsigned char *)malloc(2 * cl + 4);
        size_t nl = 0;
        size_t i = 0;
        for (; i + 1 < cl; i += 2) {
            unsigned char x = cur[i], y = cur[i + 1];
            if (x == 0 && y == 0) {
                nx[nl++] = 0; nx[nl++] = 0; nx[nl++] = 1; nx[nl++] = 0;
            } else if (x == 0 && y == 1) {
                nx[nl++] = 0; nx[nl++] = 1; nx[nl++] = 0;
            } else if (x == 1 && y == 0) {
                nx[nl++] = 0; nx[nl++] = 0; nx[nl++] = 1;
            } else {
                nx[nl++] = x; nx[nl++] = y;
            }
        }
        if (cl & 1) nx[nl++] = cur[cl - 1];
        free(cur);
        cur = nx;
        cl = nl;
    }
    *outlen = cl;
    return cur;
}

seq_t a(idx_t n)
{
    static seq_t *pos = NULL;
    static idx_t cnt = 0;

    if (n == 0) return 0;
    if (pos == NULL || n > cnt) {
        idx_t target = n * 2;
        if (target < 64) target = 64;
        size_t len = 0;
        unsigned char *bits = A289026_build_bits((size_t)(3 * target + 16), &len);
        size_t safe = len - len / 4;
        if (pos) mem_free(pos);
        pos = mem_new(seq_t, safe + 1);
        idx_t c = 0;
        for (size_t i = 0; i < safe; i++) {
            if (bits[i] == 0) {
                pos[c++] = (seq_t)(i + 1);
            }
        }
        cnt = c;
        free(bits);
    }
    return pos[n - 1];
}
