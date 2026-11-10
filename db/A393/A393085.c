/*
 * A393085  Irregular table T(n,k) where row n lists exponents m such that
 *          A001597(n) = r^m for some integer r.
 * https://oeis.org/A393085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   row r of T lists the divisors m>1 of g, where A001597(r)=b^g
 *            with b not a perfect power (row 1 is [1]); a(n) is the n-th term
 *            of the flattened rows. Computed by generating perfect powers up
 *            to a limit, sorting them, and walking the rows
 * Range:     u8 n <= 10183, u16 n <= 10183, u32 n <= 10183, u64 n <= 10183,
 *            u128 n <= 10183, big any n
 * Validated: standard, pass, 103 terms through n = 9668 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct { uint64_t p; uint32_t e; } a393085_pe;

static int a393085_cmp(const void *x, const void *y) {
    uint64_t a1 = ((const a393085_pe *)x)->p, b1 = ((const a393085_pe *)y)->p;
    return (a1 > b1) - (a1 < b1);
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    uint64_t L = 64;
    for (;;) {
        uint64_t sq = 1;
        while (sq * sq <= L) sq++;
        char *isp = (char *)calloc((size_t)sq + 2, 1);
        size_t cap = 1024, cnt = 0;
        a393085_pe *v = (a393085_pe *)malloc(cap * sizeof(a393085_pe));
        for (uint64_t b = 2; b * b <= L; b++) {
            if (isp[b]) continue;
            uint64_t p = b * b;
            uint32_t e = 2;
            while (true) {
                if (b <= sq && p <= sq) isp[p] = 1;
                if (cnt == cap) {
                    cap *= 2;
                    v = (a393085_pe *)realloc(v, cap * sizeof(a393085_pe));
                }
                v[cnt].p = p; v[cnt].e = e; cnt++;
                if (p > L / b) break;
                p *= b; e++;
            }
        }
        free(isp);
        qsort(v, cnt, sizeof(a393085_pe), a393085_cmp);
        uint64_t cum = 1;
        for (size_t i = 0; i < cnt; i++) {
            uint32_t g = v[i].e;
            uint64_t len = 0;
            for (uint32_t d = 2; d <= g; d++) if (g % d == 0) len++;
            if ((uint64_t)n <= cum + len) {
                uint64_t k = (uint64_t)n - cum;
                for (uint32_t d = 2; d <= g; d++) {
                    if (g % d == 0) {
                        if (--k == 0) { free(v); return (seq_t)d; }
                    }
                }
            }
            cum += len;
        }
        free(v);
        if (cum >= (uint64_t)n) return (seq_t)1; /* unreachable */
        L *= 4;
    }
}
