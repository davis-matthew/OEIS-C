/*
 * A245602  Triangle read by rows: the negative terms of A163626.
 * https://oeis.org/A245602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = T(r,k) for the negative entries of A163626, where *
 *            T(0,0) = 1, T(r,k) = (k+1)*T(r-1,k) - k*T(r-1,k-1); * row r >= 1
 *            contributes floor((r+1)/2) terms, at k = 1, 3, 5, ... * (T(r,k)
 *            = (-1)^k * k! * S2(r+1,k+1))
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = 1;
    for (;;) {
        idx_t cnt = (r + 1) / 2;
        if (n < cnt) break;
        n -= cnt;
        r++;
    }
    idx_t k = 2 * n + 1;

    seq_t *T = mem_new(seq_t, (size_t)(r + 2));
    T[0] = 1;
    for (idx_t m = 1; m <= r; m++) {
        T[m] = 0;
        for (idx_t j = m; j >= 1; j--) {
            T[j] = (seq_t)(j + 1) * T[j] - (seq_t)j * T[j - 1];
        }
    }
    seq_t res = T[k];
    mem_free(T);
    return res;
}
