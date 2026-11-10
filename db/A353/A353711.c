/*
 * A353711  Smallest missing number when A097465(n) is being calculated.
 * https://oeis.org/A353711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer not among the first n-1 terms
 *            of A097465 (A097465: a(1)=1; a(n) = least unused positive
 *            integer coprime to a(n-1) and != a(n-1)+-1); computed by
 *            simulation
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8881 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static unsigned long long A353711_g(unsigned long long x, unsigned long long y) {
    while (y) { unsigned long long t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n) {
    static seq_t *mexarr = NULL;     /* mexarr[i] = mex of first i terms */
    static size_t mex_cap = 0;
    static size_t cnt = 0;           /* terms of A097465 generated */
    static size_t mexv = 1;
    static size_t last = 0;
    static uint8_t *used = NULL;
    static size_t used_cap = 0;

    if (mexarr == NULL) {
        mex_cap = 1024;
        mexarr = mem_new(seq_t, mex_cap);
        mexarr[0] = (seq_t)1;
        used_cap = 65536;
        used = (uint8_t *)calloc(used_cap, 1);
        cnt = 0;
        mexv = 1;
    }

    size_t need = (size_t)(n - 1);
    while (cnt < need) {
        size_t term;
        if (cnt == 0) {
            term = 1;
        } else {
            size_t c = mexv;
            for (;;) {
                while (c + 2 >= used_cap) {
                    size_t nc = used_cap * 2;
                    uint8_t *t = (uint8_t *)realloc(used, nc);
                    memset(t + used_cap, 0, nc - used_cap);
                    used = t;
                    used_cap = nc;
                }
                if (!used[c] && c != last + 1 && c + 1 != last &&
                    A353711_g((unsigned long long)c, (unsigned long long)last) == 1)
                    break;
                ++c;
            }
            term = c;
        }
        while (term + 2 >= used_cap) {
            size_t nc = used_cap * 2;
            uint8_t *t = (uint8_t *)realloc(used, nc);
            memset(t + used_cap, 0, nc - used_cap);
            used = t;
            used_cap = nc;
        }
        used[term] = 1;
        last = term;
        while (mexv + 1 >= used_cap) {
            size_t nc = used_cap * 2;
            uint8_t *t = (uint8_t *)realloc(used, nc);
            memset(t + used_cap, 0, nc - used_cap);
            used = t;
            used_cap = nc;
        }
        while (used[mexv]) {
            ++mexv;
            if (mexv + 1 >= used_cap) {
                size_t nc = used_cap * 2;
                uint8_t *t = (uint8_t *)realloc(used, nc);
                memset(t + used_cap, 0, nc - used_cap);
                used = t;
                used_cap = nc;
            }
        }
        ++cnt;
        if (cnt >= mex_cap) {
            mex_cap *= 2;
            mexarr = mem_resize(seq_t, mexarr, mex_cap);
        }
        mexarr[cnt] = (seq_t)mexv;
    }
    return mexarr[need];
}
