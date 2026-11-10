/*
 * A284478  a(n) = A005185(n) - A055748(n).
 * https://oeis.org/A284478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A005185(n) - A055748(n), where A005185: a(1)=a(2)=1,
 *            a(n)=a(n-a(n-1))+a(n-a(n-2)); A055748: a(1)=a(2)=1,
 *            a(n)=a(a(n-1))+a(n-a(n-2)-1)
 * Range:     i8 n <= 683, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 9549 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *q_memo = NULL;
static idx_t q_len = 0;   /* values valid for indices 1..q_len */

static seq_t *c_memo = NULL;
static idx_t c_len = 0;

static seq_t A005185_a(idx_t n) {
    if (n <= 2) return 1;
    if (n > q_len) {
        seq_t *p = mem_new(seq_t, (size_t)(n + 1));
        if (p == NULL) abort();
        idx_t i;
        for (i = 1; i <= q_len; i++) p[i] = q_memo[i];
        if (q_memo != NULL) mem_free(q_memo);
        q_memo = p;
        if (q_len < 2) {
            q_memo[1] = 1;
            q_memo[2] = 1;
            q_len = 2;
        }
        for (i = q_len + 1; i <= n; i++) {
            idx_t i1 = i - (idx_t)q_memo[i - 1];
            idx_t i2 = i - (idx_t)q_memo[i - 2];
            q_memo[i] = q_memo[i1] + q_memo[i2];
        }
        q_len = n;
    }
    return q_memo[n];
}

static seq_t A055748_a(idx_t n) {
    if (n <= 2) return 1;
    if (n > c_len) {
        seq_t *p = mem_new(seq_t, (size_t)(n + 1));
        if (p == NULL) abort();
        idx_t i;
        for (i = 1; i <= c_len; i++) p[i] = c_memo[i];
        if (c_memo != NULL) mem_free(c_memo);
        c_memo = p;
        if (c_len < 2) {
            c_memo[1] = 1;
            c_memo[2] = 1;
            c_len = 2;
        }
        for (i = c_len + 1; i <= n; i++) {
            idx_t i1 = (idx_t)c_memo[i - 1];
            idx_t i2 = i - (idx_t)c_memo[i - 2] - 1;
            c_memo[i] = c_memo[i1] + c_memo[i2];
        }
        c_len = n;
    }
    return c_memo[n];
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    return A005185_a(n) - A055748_a(n);
}
