/*
 * A365231  Inverse permutation to A365230.
 * https://oeis.org/A365231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A365230 (inverse permutation); computed
 *            by generating A365230 greedily (T(n,1)=least unused,
 *            T(n,k)=least unused multiple of T(k,1)) until n appears
 * Range:     u8 n <= 82, u16 n <= 2440, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8587 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static void A365231_grow_pos(idx_t m, idx_t **pos, size_t *cap)
{
    if ((size_t)m >= *cap) {
        size_t nc = (*cap > 0) ? *cap : 16;
        while (nc <= (size_t)m) nc <<= 1;
        idx_t *t = (idx_t *)realloc(*pos, nc * sizeof(idx_t));
        if (!t) exit(1);
        memset(t + *cap, 0, (nc - *cap) * sizeof(idx_t));
        *pos = t;
        *cap = nc;
    }
}

static void A365231_grow_rows(size_t need, idx_t **fv, idx_t **nm, size_t *cap)
{
    if (need >= *cap) {
        size_t nc = (*cap > 0) ? *cap : 16;
        while (nc <= need) nc <<= 1;
        idx_t *t1 = (idx_t *)realloc(*fv, nc * sizeof(idx_t));
        if (!t1) exit(1);
        *fv = t1;
        idx_t *t2 = (idx_t *)realloc(*nm, nc * sizeof(idx_t));
        if (!t2) exit(1);
        *nm = t2;
        *cap = nc;
    }
}

seq_t a(idx_t n)
{
    static idx_t *pos = NULL;      static size_t posCap = 0;
    static idx_t *firstVal = NULL; static idx_t *nextMult = NULL; static size_t rowCap = 0;
    static idx_t row = 0, col = 1, count = 0;
    static bool init = false;

    if (n == 0) return 0;

    if (!init) {
        A365231_grow_pos(16, &pos, &posCap);
        A365231_grow_rows(16, &firstVal, &nextMult, &rowCap);
        firstVal[1] = 1;
        nextMult[1] = 1;
        init = true;
    }

    while ((size_t)n >= posCap || pos[(size_t)n] == 0) {
        if (col > row) {
            row++;
            col = 1;
            A365231_grow_rows((size_t)row, &firstVal, &nextMult, &rowCap);
        }
        idx_t base = firstVal[(size_t)col];
        idx_t cand = nextMult[(size_t)col];
        for (;;) {
            A365231_grow_pos(cand, &pos, &posCap);
            if (!pos[(size_t)cand]) break;
            cand += base;
        }
        count++;
        pos[(size_t)cand] = count;
        nextMult[(size_t)col] = cand + base;
        if (col == 1) {
            firstVal[(size_t)row] = cand;
            nextMult[(size_t)row] = cand + cand;
        }
        col++;
    }
    return (seq_t)pos[(size_t)n];
}
