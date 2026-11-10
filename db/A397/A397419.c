/*
 * A397419  a(n) = Sum_{j=0..n} binomial(n, j) * A005043(j)^2.
 * https://oeis.org/A397419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} binomial(n, j) * A005043(j)^2, with
 *            A005043(n) = (n-1)*(2*A005043(n-1) + 3*A005043(n-2))/(n+1),
 *            A005043(0) = 1, A005043(1) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A005043_grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *A005043_memo = NULL;
static idx_t A005043_memo_len = 0;

static seq_t A005043_a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;

    if (A005043_memo_len == 0) {
        A005043_memo = A005043_grow_zero(A005043_memo, 0, 2);
        A005043_memo[0] = 1;
        A005043_memo[1] = 0;
        A005043_memo_len = 2;
    }
    if (n >= A005043_memo_len) {
        A005043_memo = A005043_grow_zero(A005043_memo, A005043_memo_len, n + 1);
        for (idx_t i = A005043_memo_len; i <= n; i++) {
            idx_t g = (i & 1) ? 2 : 1;
            seq_t p = (seq_t)((i - 1) / g);
            seq_t m = (seq_t)((i + 1) / g);
            seq_t q1 = A005043_memo[i - 1] / m, r1 = A005043_memo[i - 1] % m;
            seq_t q2 = A005043_memo[i - 2] / m, r2 = A005043_memo[i - 2] % m;
            seq_t q = 2 * q1 + 3 * q2 + (2 * r1 + 3 * r2) / m;
            A005043_memo[i] = p * q;
        }
        A005043_memo_len = n + 1;
    }
    return A005043_memo[n];
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t c = 1; /* binomial(n, j) */
    for (idx_t j = 0; j <= n; j++) {
        seq_t r = A005043_a(j);
        sum += c * r * r;
        if (j < n) {
            seq_t d = (seq_t)j + 1;
            seq_t f = (seq_t)(n - j);
            seq_t q = c / d, rem = c % d;
            c = q * f + (rem * f) / d;
        }
    }
    return sum;
}
