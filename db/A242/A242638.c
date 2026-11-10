/*
 * A242638  Number of quadrangulations of a hexagon b_1 w_3 b_2 w_1 b_3 w_2
 *          having n inner faces and not containing the edge b_1 w_1.
 * https://oeis.org/A242638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   g.f. (2 - 6*g + 3*g^2)*g^2/(1-g)^4 where g = x*(1 + 2x + 7x^2 +
 *            ...) = x*C(x), C = 1 + x*C^3 (A006013 shifted); series computed
 *            by truncated convolutions
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 475 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static void A242638_mul(const seq_t *p, const seq_t *q, seq_t *r, size_t N) {
    size_t i, j;
    for (i = 0; i < N; i++) {
        seq_t s = 0;
        for (j = 0; j <= i; j++) s += p[j] * q[i - j];
        r[i] = s;
    }
}

seq_t a(idx_t n) {
    size_t N = (size_t)n + 1, k, i;
    seq_t *c = mem_new(seq_t, N);
    seq_t *c2 = mem_new(seq_t, N);
    seq_t *c3 = mem_new(seq_t, N);
    seq_t *g = mem_new(seq_t, N);
    seq_t *S = mem_new(seq_t, N);
    seq_t *S2 = mem_new(seq_t, N);
    seq_t *D = mem_new(seq_t, N);
    seq_t *P2 = mem_new(seq_t, N);
    seq_t *P3 = mem_new(seq_t, N);
    seq_t *P4 = mem_new(seq_t, N);
    seq_t pos = 0, neg = 0, res;

    /* c = 1 + x*c^3 */
    for (k = 0; k < N; k++) {
        seq_t s2 = 0, s3 = 0;
        if (k == 0) c[0] = 1; else c[k] = c3[k - 1];
        for (i = 0; i <= k; i++) s2 += c[i] * c[k - i];
        c2[k] = s2;
        for (i = 0; i <= k; i++) s3 += c2[i] * c[k - i];
        c3[k] = s3;
    }
    g[0] = 0;
    for (k = 1; k < N; k++) g[k] = c[k - 1 + 0] * 0 + c[k];
    /* g = x*(1+2x+7x^2+...) : g[k] = A006013(k-1) = c[k]?  c = 1,1,3,... so use shift below */
    /* c itself is 1 + x c^3 -> 1,1,3,12,...; the needed series is g with g = x*c^... fix: g = c - 1 shifted */
    /* Recompute g properly: g/x = 1,2,7,30,... = c^2-type; g = c - 1 satisfies g = x(1+g)^3 */
    /* Here c - 1 = x + 3x^2 + ... differs, so derive g from A006013 = [x^k] c^2 */
    for (k = 0; k < N; k++) {
        /* c2[k] = [x^k] c^2 = binom(3k+2,k)*2/(3k+2) = 1,2,7,30,... = A006013(k) */
        g[k] = (k == 0) ? (seq_t)0 : c2[k - 1];
    }

    /* S = 1/(1-g) */
    for (k = 0; k < N; k++) {
        seq_t s = (k == 0) ? 1 : 0;
        for (i = 1; i <= k; i++) s += g[i] * S[k - i];
        S[k] = s;
    }
    A242638_mul(S, S, S2, N);
    A242638_mul(S2, S2, D, N);
    A242638_mul(g, g, P2, N);
    A242638_mul(P2, g, P3, N);
    A242638_mul(P3, g, P4, N);

    for (i = 0; i < N; i++) {
        pos += (2 * P2[i] + 3 * P4[i]) * D[(size_t)n - i];
        neg += 6 * P3[i] * D[(size_t)n - i];
    }
    res = pos - neg;

    mem_free(c); mem_free(c2); mem_free(c3); mem_free(g);
    mem_free(S); mem_free(S2); mem_free(D);
    mem_free(P2); mem_free(P3); mem_free(P4);
    return res;
}
