/*
 * A349346  Dirichlet inverse of A181988, where A181988(n) =
 *          A001511(n)*A003602(n).
 * https://oeis.org/A349346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = -Sum_{d|n, d<n} A181988(n/d) * a(d), where
 *            A181988(n) = A001511(n)*A003602(n)
 * Range:     i8 n <= 252, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 8767 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static seq_t A181988_a(idx_t n) {
    idx_t v = 0;
    while ((n & 1ULL) == 0ULL) {
        n >>= 1;
        v++;
    }
    seq_t part = ((seq_t)n + 1) / 2;
    return part * (seq_t)(v + 1);
}

seq_t a(idx_t n) {
    seq_t *arr = mem_new(seq_t, (size_t)n + 1);
    idx_t i, j;
    seq_t res;
    for (i = 1; i <= n; i++) {
        if (i == 1) arr[i] = 1;
        else arr[i] = -arr[i];
        if (i * 2 <= n) {
            for (j = 2 * i; j <= n; j += i) {
                arr[j] += A181988_a(j / i) * arr[i];
            }
        }
    }
    res = arr[n];
    mem_free(arr);
    return res;
}
