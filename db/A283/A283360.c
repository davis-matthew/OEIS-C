/*
 * A283360  Absolute values of first differences of A283025.
 * https://oeis.org/A283360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(A283025(n+1) - A283025(n)), where A283025(n) =
 *            (Sum_{k=1..n} A005185(k)) mod n, A005185(1)=A005185(2)=1,
 *            A005185(k)=A005185(k-A005185(k-1))+A005185(k-A005185(k-2))
 * Range:     u8 n <= 27, u16 n <= 526, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9291 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *q_memo = NULL;
static idx_t q_len = 0;

static seq_t A005185_a(idx_t n) {
    if (n <= 0) return 0;
    if (n <= 2) return 1;
    if (n >= q_len) {
        idx_t old_len = q_len;
        idx_t new_len = n + 1;
        seq_t *p = mem_new(seq_t, (size_t)new_len);
        if (old_len > 0) {
            mem_copy(p, q_memo, (size_t)old_len * sizeof(seq_t));
            mem_free(q_memo);
        }
        q_memo = p;
        if (old_len < 3) {
            q_memo[1] = 1;
            q_memo[2] = 1;
            old_len = 3;
        }
        for (idx_t i = old_len; i <= n; i++) {
            idx_t i1 = i - (idx_t)q_memo[i - 1];
            idx_t i2 = i - (idx_t)q_memo[i - 2];
            q_memo[i] = q_memo[i1] + q_memo[i2];
        }
        q_len = new_len;
    }
    return q_memo[n];
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t s = 0;
    seq_t r1 = 0;
    for (idx_t k = 1; k <= n; k++) {
        s += A005185_a(k);
    }
    r1 = s % (seq_t)n;
    s += A005185_a(n + 1);
    seq_t r2 = s % (seq_t)(n + 1);
    if (r2 >= r1) return r2 - r1;
    return r1 - r2;
}
