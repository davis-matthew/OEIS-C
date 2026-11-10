/*
 * A341323  a(n) = Sum_{k=1..n+1} |A341287(n,k)|.
 * https://oeis.org/A341323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n+1} |T(n,k)|, T(0,1)=1, T(n,k) =
 *            Sum_{i=0..n-1} C(n-1,i)*T(n-1-i,k-1) - Sum_{i=1..n-1}
 *            C(n-1,i)*T(n-1-i,k) (A341287)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-10)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t **A341323_M = NULL;        /* magnitudes of T(row,k) */
static signed char **A341323_S = NULL;  /* 1 if negative */
static size_t A341323_cnt = 0;          /* rows computed */

static void A341323_add(seq_t *m, signed char *s, seq_t x, signed char sx) {
    if (x == 0) return;
    if (*m == 0) { *m = x; *s = sx; return; }
    if (*s == sx) { *m += x; }
    else if (*m >= x) { *m -= x; if (*m == 0) *s = 0; }
    else { *m = x - *m; *s = sx; }
}

static void A341323_ensure(size_t n) {
    if (A341323_cnt == 0) {
        A341323_M = (seq_t**)malloc(sizeof(seq_t*));
        A341323_S = (signed char**)malloc(sizeof(signed char*));
        if (!A341323_M || !A341323_S) abort();
        A341323_M[0] = mem_new(seq_t, 2);
        A341323_S[0] = (signed char*)calloc(2, 1);
        A341323_M[0][1] = 1;
        A341323_cnt = 1;
    }
    while (A341323_cnt <= n) {
        size_t r = A341323_cnt;
        A341323_M = (seq_t**)realloc(A341323_M, (r + 1) * sizeof(seq_t*));
        A341323_S = (signed char**)realloc(A341323_S, (r + 1) * sizeof(signed char*));
        if (!A341323_M || !A341323_S) abort();
        seq_t *rm = mem_new(seq_t, r + 2);
        signed char *rs = (signed char*)calloc(r + 2, 1);
        if (!rs) abort();
        seq_t *bins = mem_new(seq_t, r + 1);
        for (size_t i = 0; i < r; i++) bins[i] = binomial((idx_t)(r - 1), (idx_t)i);
        for (size_t k = 1; k <= r + 1; k++) {
            seq_t m = 0;
            signed char s = 0;
            for (size_t i = 0; i < r; i++) {
                size_t r1 = r - 1 - i;
                size_t c1 = k - 1;
                if (c1 >= 1 && c1 <= r1 + 1) {
                    A341323_add(&m, &s, bins[i] * A341323_M[r1][c1], A341323_S[r1][c1]);
                }
                if (i >= 1 && k <= r1 + 1) {
                    A341323_add(&m, &s, bins[i] * A341323_M[r1][k], (signed char)(A341323_S[r1][k] ^ 1));
                }
            }
            rm[k] = m;
            rs[k] = s;
        }
        mem_free(bins);
        A341323_M[r] = rm;
        A341323_S[r] = rs;
        A341323_cnt = r + 1;
    }
}

seq_t a(idx_t n) {
    A341323_ensure((size_t)n);
    seq_t sum = 0;
    for (size_t k = 1; k <= (size_t)n + 1; k++) sum += A341323_M[n][k];
    return sum;
}
