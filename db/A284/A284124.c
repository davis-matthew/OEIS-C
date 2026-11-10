/*
 * A284124  Remainder when 4*n is divided by A005185(n).
 * https://oeis.org/A284124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (4*n) mod Q(n), where Q = A005185: Q(1)=Q(2)=1, Q(n) =
 *            Q(n-Q(n-1)) + Q(n-Q(n-2))
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9613 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A005185_memo = NULL;
static idx_t A005185_len = 0;

static seq_t A005185_a(idx_t n) {
    if (n <= 0) return 0;
    if (n <= 2) return 1;
    if (n >= A005185_len) {
        idx_t newlen = n + 1;
        seq_t *p = mem_new(seq_t, (size_t)newlen);
        idx_t start = 3;
        if (A005185_memo != NULL) {
            mem_copy(p, A005185_memo, (size_t)A005185_len * sizeof(seq_t));
            mem_free(A005185_memo);
            start = A005185_len;
            if (start < 3) start = 3;
        }
        p[1] = 1;
        p[2] = 1;
        A005185_memo = p;
        for (idx_t i = start; i <= n; i++) {
            idx_t i1 = i - (idx_t)A005185_memo[i - 1];
            idx_t i2 = i - (idx_t)A005185_memo[i - 2];
            A005185_memo[i] = A005185_memo[i1] + A005185_memo[i2];
        }
        A005185_len = newlen;
    }
    return A005185_memo[n];
}

seq_t a(idx_t n) {
    seq_t q = A005185_a(n);
    seq_t m = (seq_t)n * 4;
    return m % q;
}
