/*
 * A397603  Triangle read by rows: T(n, k) = T(n, k+1) + T(n-1, k) with
 *          initial values T(n, n) = A005043(n)^2.
 * https://oeis.org/A397603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n,k+1) + T(n-1,k), T(n,n) = A005043(n)^2, T(0,0)=1;
 *            a(n) is the triangle read by rows, 0 <= k <= row
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-10)
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

seq_t a(idx_t m) {
    idx_t row = 0;
    idx_t start = 0;
    while (start + row + 1 <= m) {
        start += row + 1;
        row++;
    }
    idx_t col = m - start;

    seq_t *prev = mem_new(seq_t, (size_t)row + 1);
    seq_t *cur = mem_new(seq_t, (size_t)row + 1);
    prev[0] = 1;
    for (idx_t r = 1; r <= row; r++) {
        seq_t v = A005043_a(r);
        cur[r] = v * v;
        for (idx_t k = r; k-- > 0;) {
            cur[k] = cur[k + 1] + prev[k];
        }
        seq_t *t = prev; prev = cur; cur = t;
    }
    seq_t res = prev[col];
    mem_free(prev);
    mem_free(cur);
    return res;
}
