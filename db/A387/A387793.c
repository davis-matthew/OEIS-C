/*
 * A387793  a(n) = Sum_{k=1..floor(n/2)} A389524(n, n - 2*k + 1).
 * https://oeis.org/A387793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} T(n, n-2k+1), where T is A389524:
 *            T(1,1)=1, T(m,k)=[m>=k]*(1 + Sum_{i=1..k-1} T(m-i,k-1) -
 *            Sum_{i=1..m-1} T(m-i,k)). Computed with column prefix sums,
 *            O(n^2)
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;
    size_t W = N + 1;
    /* S[m*W+k] = sum_{r<=m} T(r,k) */
    seq_t *S = mem_new(seq_t, W * W);
    seq_t *Trow = mem_new(seq_t, W);
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = 1; k <= m; ++k) {
            seq_t t = (seq_t)1;
            if (k >= 2) {
                t += S[(m - 1) * W + (k - 1)] - S[(m - k) * W + (k - 1)];
            }
            t -= S[(m - 1) * W + k];
            Trow[k] = t;
        }
        for (size_t k = 1; k <= N; ++k) {
            seq_t prev = S[(m - 1) * W + k];
            S[m * W + k] = (k <= m) ? (prev + Trow[k]) : prev;
        }
    }
    seq_t res = 0;
    for (size_t k = 1; 2 * k <= N; ++k) {
        res += Trow[N - 2 * k + 1];
    }
    mem_free(S);
    mem_free(Trow);
    return res;
}
