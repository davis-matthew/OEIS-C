/*
 * A292130  Triangle read by rows: T(n,k) = (-3) * T(n-k,k-1) + T(n-k,k) with
 *          T(0,0) = 1 for 0 <= k <= A003056(n).
 * https://oeis.org/A292130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   T(n,k) = -3*T(n-k,k-1) + T(n-k,k), T(0,0) = 1, T(n,0) = 0 for n
 *            > 0, 0 <= k <= A003056(n); a(n) is the triangle read by rows
 * Range:     i8 n <= 37, i16 n <= 184, i32 n <= 968, i64 n <= 5958,
 *            i128 n <= 9981, big any n
 * Validated: standard, pass, 81 terms through n = 9977 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* locate row R and column C of flat index n */
    idx_t R = 0, pos = 0, r = 0;
    for (;;) {
        while ((r + 1) * (r + 2) / 2 <= R) r++;
        if (n < pos + r + 1) break;
        pos += r + 1;
        R++;
    }
    idx_t C = n - pos;
    idx_t K = r;
    size_t W = (size_t)K + 1;
    seq_t *T = mem_new(seq_t, ((size_t)R + 1) * W);
    T[0] = 1;
    for (idx_t m = 1; m <= R; m++) {
        for (idx_t k = 1; k <= K && k <= m; k++) {
            seq_t v = (seq_t)0;
            v = v - (seq_t)3 * T[(size_t)(m - k) * W + (size_t)(k - 1)];
            v = v + T[(size_t)(m - k) * W + (size_t)k];
            T[(size_t)m * W + (size_t)k] = v;
        }
    }
    seq_t res = T[(size_t)R * W + (size_t)C];
    mem_free(T);
    return res;
}
