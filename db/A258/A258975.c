/*
 * A258975  a(n) = n-th pi-based antiderivative of 1.
 * https://oeis.org/A258975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min { m >= 0 : A258851^n(m) = 1 }, where A258851(p) =
 *            pi(p) * for p prime and A258851(u*v) = A258851(u)*v +
 *            u*A258851(v). * Search by sieve: compute d(x) = A258851(x) =
 *            Sum_{p^k | x} pi(p)*x/p for all * x <= S, then the number of
 *            steps to reach 1 for every x <= S (memoized * chain walk, ch...
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define A258975_UNK  255u
#define A258975_PROG 254u
#define A258975_INF  253u
#define A258975_MAXR 253

static uint64_t A258975_best[A258975_MAXR];
static uint64_t A258975_curS = 0;

static void A258975_compute(uint64_t S) {
    uint32_t *D = (uint32_t *)calloc((size_t)(S + 1), sizeof(uint32_t));
    uint8_t *st = (uint8_t *)malloc((size_t)(S + 1));
    if (!D || !st) { free(D); free(st); return; }

    /* d(x) = sum over prime powers q = p^k dividing x of pi(p) * (x/p) */
    uint64_t pi = 0;
    for (uint64_t p = 2; p <= S; p++) {
        if (D[p] != 0) continue;           /* composite: already touched */
        pi++;
        for (uint64_t q = p; q <= S; q *= p) {
            uint64_t step_t = q / p;
            uint64_t t = step_t;
            for (uint64_t x = q; x <= S; x += q, t += step_t) {
                uint64_t v = (uint64_t)D[x] + pi * t;
                D[x] = (v > 0xFFFFFFFFull) ? 0xFFFFFFFFu : (uint32_t)v;
            }
            if (q > S / p) break;
        }
    }

    memset(st, (int)A258975_UNK, (size_t)(S + 1));
    st[0] = (uint8_t)A258975_INF;
    if (S >= 1) st[1] = 0;

    size_t cap = 1024, top = 0;
    uint32_t *stk = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!stk) { free(D); free(st); return; }

    for (uint64_t v = 2; v <= S; v++) {
        if (st[v] != A258975_UNK) continue;
        top = 0;
        stk[top++] = (uint32_t)v;
        st[v] = (uint8_t)A258975_PROG;
        uint64_t w = D[v];
        unsigned val;
        for (;;) {
            if (w > S) { val = A258975_INF; break; }
            unsigned sw = st[w];
            if (sw == A258975_PROG) { val = A258975_INF; break; }
            if (sw != A258975_UNK) { val = sw; break; }
            if (top == cap) {
                cap *= 2;
                uint32_t *ns = (uint32_t *)realloc(stk, cap * sizeof(uint32_t));
                if (!ns) { free(stk); free(D); free(st); return; }
                stk = ns;
            }
            stk[top++] = (uint32_t)w;
            st[w] = (uint8_t)A258975_PROG;
            w = D[w];
        }
        while (top > 0) {
            uint32_t x = stk[--top];
            if (val != A258975_INF) {
                val = val + 1;
                if (val >= A258975_INF) val = A258975_INF;
            }
            st[x] = (uint8_t)val;
        }
    }
    free(stk);

    for (int r = 0; r < A258975_MAXR; r++) A258975_best[r] = 0;
    for (uint64_t m = 1; m <= S; m++) {
        unsigned r = st[m];
        if (r < A258975_INF && A258975_best[r] == 0) A258975_best[r] = m;
    }
    free(D);
    free(st);
    A258975_curS = S;
}

seq_t a(idx_t n) {
    if (n >= (idx_t)A258975_MAXR) return (seq_t)0;
    for (;;) {
        if (A258975_curS > 0 && A258975_best[n] != 0 &&
            A258975_best[n] <= A258975_curS / 4)
            return (seq_t)A258975_best[n];
        uint64_t S = A258975_curS ? A258975_curS * 2 : ((uint64_t)1 << 16);
        uint64_t prev = A258975_curS;
        A258975_compute(S);
        if (A258975_curS == prev) {
            /* allocation failed: return best known value if any */
            return (seq_t)A258975_best[n];
        }
    }
}
