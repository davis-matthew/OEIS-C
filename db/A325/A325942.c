/*
 * A325942  Positions of records in A296062.
 * https://oeis.org/A325942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position (index, offset 0) of the n-th strict record of
 *            A296062, where A296062: b(0)=b(1)=0, b(2k)=b(k)+b(k-1)+1,
 *            b(2k+1)=2*b(k); computed by scanning
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 984 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t cap = 0;
    static size_t cnt = 0;          /* number of b values computed */
    static uint64_t *rec = NULL;
    static size_t rcap = 0;
    static size_t rcnt = 0;
    static seq_t best = 0;

    if (n < 1) return 0;
    while (rcnt < (size_t)n) {
        size_t i = cnt;
        if (i >= cap) {
            size_t nc = cap ? cap * 2 : 64;
            seq_t *t = mem_resize(seq_t, vals, nc);
            if (!t) return 0;
            vals = t;
            cap = nc;
        }
        seq_t v;
        if (i < 2) v = 0;
        else if ((i & 1) == 0) v = vals[i >> 1] + vals[(i >> 1) - 1] + 1;
        else v = vals[i >> 1] * 2;
        vals[i] = v;
        cnt = i + 1;
        if (i == 0 || v > best) {
            best = v;
            if (rcnt >= rcap) {
                size_t nr = rcap ? rcap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(rec, nr * sizeof(uint64_t));
                if (!t) return 0;
                rec = t;
                rcap = nr;
            }
            rec[rcnt++] = (uint64_t)i;
        }
    }
    return (seq_t)rec[(size_t)n - 1];
}
