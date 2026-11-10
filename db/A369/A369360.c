/*
 * A369360  a(n) is the number of interior points over all Motzkin polyominoes
 *          of length n.
 * https://oeis.org/A369360
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^n - 3*A002426(n))/2 - 2*A002426(n-1) + 2*A001006(n-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 30 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A002426_a(idx_t n) {
    seq_t p2 = 1, p1 = 1;
    idx_t i;
    if (n <= 1) return 1;
    for (i = 2; i <= n; i++) {
        seq_t N = (seq_t)i;
        seq_t q1, r1, q2, r2, c1, c2, cur;
        q1 = p1 / N; r1 = p1 % N;
        q2 = p2 / N; r2 = p2 % N;
        c1 = N * 2 - 1;
        c2 = (N - 1) * 3;
        cur = c1 * q1 + c2 * q2 + (c1 * r1 + c2 * r2) / N;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}

static seq_t A001006_a(idx_t n) {
    if (n <= 1) return 1;
    seq_t *m = mem_new(seq_t, (size_t)n + 1);
    m[0] = 1;
    m[1] = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t sum = 0;
        for (idx_t k = 0; k + 2 <= i; k++) {
            sum = sum + m[k] * m[i - 2 - k];
        }
        m[i] = m[i - 1] + sum;
    }
    seq_t r = m[n];
    mem_free(m);
    return r;
}

seq_t a(idx_t n) {
    seq_t p3 = 1;
    for (idx_t i = 0; i + 1 < n; i++) p3 = p3 * 3; /* 3^(n-1) */
    seq_t c = A002426_a(n);
    seq_t c1 = A002426_a(n - 1);
    seq_t M = A001006_a(n - 1);
    seq_t D = p3 - c;
    seq_t res = (D / 2) * 3 + M * 2;
    res = res - c1 * 2;
    return res;
}
