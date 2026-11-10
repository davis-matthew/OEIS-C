/*
 * A325931  Signs of first differences of A076042.
 * https://oeis.org/A325931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 if A076042(n-1) < n^2 else -1, where A076042(0) = 0 and
 *            A076042(n) = A076042(n-1) + a(n)*n^2
 * Range:     i8 n <= 13, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A325931_vals = NULL;   /* A076042 values, index 0..len-1 */
static size_t A325931_len = 0;
static size_t A325931_cap = 0;

seq_t a(idx_t n) {
    while (A325931_len <= (size_t)n) {
        if (A325931_len == A325931_cap) {
            size_t nc = A325931_cap ? A325931_cap * 2 : 64;
            seq_t *t = mem_resize(seq_t, A325931_vals, nc);
            if (!t) exit(1);
            A325931_vals = t;
            A325931_cap = nc;
        }
        if (A325931_len == 0) {
            A325931_vals[0] = 0;
            A325931_len = 1;
            continue;
        }
        idx_t i = (idx_t)A325931_len;
        seq_t prev = A325931_vals[i - 1];
        seq_t sq = (seq_t)i * (seq_t)i;
        A325931_vals[i] = (prev < sq) ? (prev + sq) : (prev - sq);
        A325931_len++;
    }
    seq_t prev = A325931_vals[n - 1];
    seq_t sq = (seq_t)n * (seq_t)n;
    return (prev < sq) ? (seq_t)1 : (seq_t)-1;
}
