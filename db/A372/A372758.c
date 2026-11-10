/*
 * A372758  5th prepended column of the 3-Zeckendorf array (A136189).
 * https://oeis.org/A372758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005374(A005374(n-1)) + 1, where A005374 is H(m) = m -
 *            H(H(H(m-1))), H(0) = 0
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *H_memo = NULL;
static idx_t H_cap = 0;
static idx_t H_len = 0;

static void H_ensure(idx_t m) {
    if (H_memo == NULL || m >= H_cap) {
        idx_t nc = m + 1024;
        seq_t *nm = mem_new(seq_t, nc);
        if (H_memo != NULL) {
            for (idx_t i = 0; i < H_len; i++) nm[i] = H_memo[i];
            mem_free(H_memo);
        }
        H_memo = nm;
        H_cap = nc;
    }
    if (H_len == 0) {
        H_memo[0] = (seq_t)0;
        H_len = 1;
    }
    for (idx_t i = H_len; i <= m; i++) {
        long long p = (long long)H_memo[i - 1];
        long long q = (long long)H_memo[p];
        H_memo[i] = (seq_t)i - H_memo[q];
    }
    if (m + 1 > H_len) H_len = m + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    H_ensure(n - 1);
    long long k = (long long)H_memo[n - 1];
    return H_memo[k] + 1;
}
