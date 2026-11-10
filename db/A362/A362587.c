/*
 * A362587  a(n) = 2^n * A094088(n). Row sums of A362586.
 * https://oeis.org/A362587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * A094088(n), where A094088(0) = 1 and A094088(n) =
 *            Sum_{k=1..n} binomial(2n,2k)*A094088(n-k) (e.g.f. 1/(2-cosh(x)),
 *            even coefficients)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *row = mem_new(seq_t, 2 * N + 2);
    seq_t *A = mem_new(seq_t, N + 1);
    if (!row || !A) return 0;
    row[0] = 1;
    A[0] = 1;
    size_t r = 0; /* current Pascal row index */
    for (size_t m = 1; m <= N; ++m) {
        for (int s = 0; s < 2; ++s) {
            ++r;
            for (size_t j = r; j >= 1; --j)
                row[j] += row[j - 1];
        }
        seq_t sum = 0;
        for (size_t k = 1; k <= m; ++k)
            sum += row[2 * k] * A[m - k];
        A[m] = sum;
    }
    seq_t res = A[N];
    for (size_t i = 0; i < N; ++i)
        res = res * 2;
    mem_free(row);
    mem_free(A);
    return res;
}
