/*
 * A260593  The values of the modified Syracuse algorithm, msa, in the order
 *          in which they appear in A260590.
 * https://oeis.org/A260593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th distinct value in order of first appearance in
 *            A260590, where A260590(m) = drop time of 2m+1 under x ->
 *            (3x+1)/2 (odd) or x/2 (even); computed by scanning m = 1, 2,
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t A260590_a(idx_t n)
{
    seq_t start = (seq_t)2 * (seq_t)n + 1;
    seq_t orig  = start;
    seq_t steps = 0;
    while (start >= orig) {
        if ((start & 1) == 0) {
            start >>= 1;
        } else {
            start = (start * 3 + 1) >> 1;
        }
        ++steps;
    }
    return steps;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static unsigned char *seen = NULL;
    static size_t seen_cap = 0;
    static idx_t next_m = 1;

    if (n < 1) return 0;
    if (cap == 0) {
        cap = 64;
        terms = mem_new(seq_t, cap);
        if (!terms) { cap = 0; return 0; }
    }
    if (seen_cap == 0) {
        seen_cap = 1024;
        seen = (unsigned char *)malloc(seen_cap);
        if (!seen) { seen_cap = 0; return 0; }
        memset(seen, 0, seen_cap);
    }

    while (cnt < (size_t)n) {
        seq_t d = A260590_a(next_m);
        next_m++;
        size_t dl = (size_t)(long long)d;
        if (dl >= seen_cap) {
            size_t nc = seen_cap * 2;
            while (nc <= dl) nc *= 2;
            unsigned char *t = (unsigned char *)realloc(seen, nc);
            if (!t) return 0;
            memset(t + seen_cap, 0, nc - seen_cap);
            seen = t;
            seen_cap = nc;
        }
        if (!seen[dl]) {
            seen[dl] = 1;
            if (cnt == cap) {
                size_t nc = cap * 2;
                seq_t *t = mem_resize(seq_t, terms, nc);
                if (!t) return 0;
                terms = t;
                cap = nc;
            }
            terms[cnt++] = d;
        }
    }
    return terms[(size_t)n - 1];
}
