/*
 * A390668  Row sums of A390434.
 * https://oeis.org/A390668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), where T(n,k) = T(n-1,k-1) +
 *            (n+k-1)^3 * T(n-1,k), T(0,0) = 1, T(n,0) = 0 for n > 0, T(n,n) =
 *            1 (A390434, Lah numbers with level 3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    seq_t *T = mem_new(seq_t, N + 2);
    T[0] = (seq_t)1;
    for (size_t r = 1; r <= N; ++r) {
        for (size_t k = r; k >= 1; --k) {
            seq_t f = (seq_t)(r + k - 1);
            f = f * f * f;
            T[k] = T[k - 1] + f * T[k];
        }
        T[0] = (seq_t)0;
    }
    seq_t s = (seq_t)0;
    for (size_t k = 0; k <= N; ++k) s += T[k];
    mem_free(T);
    return s;
}
