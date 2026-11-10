/*
 * A284565  Bisection of A000360.
 * https://oeis.org/A284565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = A000360(2n), where A000360(0) = 1, A000360(2m) =
 *            A000360(m) + A000360(m-1), A000360(2m+1) = A000360(m) + 1 -
 *            ((m-1) mod 3) (with (m-1) mod 3 taken as 2 for m = 0)
 * Range:     u8 n <= 4777, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 114 terms through n = 7607 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    idx_t N = 2 * n;
    seq_t *A = mem_new(seq_t, (size_t)N + 1);
    A[0] = 1;
    for (idx_t i = 1; i <= N; i++) {
        idx_t k = i / 2;
        if (i % 2 == 0) {
            A[i] = A[k] + A[k - 1];
        } else {
            unsigned m = (k == 0) ? 2u : (unsigned)((k - 1) % 3);
            A[i] = A[k] + 1 - m;
        }
    }
    seq_t r = A[N];
    mem_free(A);
    return r;
}
