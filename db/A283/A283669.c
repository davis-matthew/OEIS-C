/*
 * A283669  a(n) = q(n-q(n+1)+2) - q(n-q(n)+2) where q(n) = A005185(n).
 * https://oeis.org/A283669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = q(n-q(n+1)+2) - q(n-q(n)+2), where q(n) = A005185(n):
 *            q(1) = q(2) = 1, q(n) = q(n-q(n-1)) + q(n-q(n-2)) for n > 2
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A283669_memo = NULL;
static idx_t A283669_len = 0;

static seq_t A005185_a(idx_t n) {
    if (n <= 0) return 0;
    if (n <= 2) return 1;
    if (n >= A283669_len) {
        idx_t newlen = n + 1;
        seq_t *p = mem_new(seq_t, (size_t)newlen);
        idx_t start = 3;
        if (A283669_memo != NULL) {
            mem_copy(p, A283669_memo, (size_t)A283669_len * sizeof(seq_t));
            mem_free(A283669_memo);
            start = A283669_len;
            if (start < 3) start = 3;
        } else {
            p[1] = 1;
            p[2] = 1;
        }
        A283669_memo = p;
        for (idx_t i = start; i <= n; i++) {
            idx_t i1 = i - (idx_t)(long long)A283669_memo[i - 1];
            idx_t i2 = i - (idx_t)(long long)A283669_memo[i - 2];
            A283669_memo[i] = A283669_memo[i1] + A283669_memo[i2];
        }
        A283669_len = newlen;
    }
    return A283669_memo[n];
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t qn = A005185_a(n);
    seq_t qn1 = A005185_a(n + 1);
    idx_t i1 = n + 2 - (idx_t)(long long)qn1;
    idx_t i2 = n + 2 - (idx_t)(long long)qn;
    return A005185_a(i1) - A005185_a(i2);
}
