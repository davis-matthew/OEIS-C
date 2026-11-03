/*
 * A303001  a(n) = permanent of the n X n matrix with entries a(i, i) =
 *          i*(i+1)/2 and a(i, j) = 1 elsewhere.
 * https://oeis.org/A303001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of n×n matrix B with
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A303001: the permanent of the n X n matrix with i(i+1)/2 on the diagonal and
 * 1 elsewhere. Summing over the set of fixed points of the permutation:
 *   a(n) = Sum_{k=0..n} e_k(d_1..d_n) D(n-k),
 * with e_k the elementary symmetric functions of d_i = i(i+1)/2 and D the
 * derangement numbers, D(m) = (m-1)(D(m-1) + D(m-2)). Ring operations only,
 * so exact modulo the width. */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    const size_t N = (size_t)n;
    seq_t *e = mem_new(seq_t, N + 1);
    seq_t *D = mem_new(seq_t, N + 1);
    if (!e || !D) abort();
    e[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) {
        seq_t d = (seq_t)(i * (i + 1) / 2);
        for (size_t k = i; k >= 1; --k) e[k] += e[k - 1] * d;
    }
    D[0] = (seq_t)1;
    if (N >= 1) D[1] = (seq_t)0;
    for (size_t m = 2; m <= N; ++m) D[m] = (seq_t)(m - 1) * (D[m - 1] + D[m - 2]);
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k <= N; ++k) sum += e[k] * D[N - k];
    mem_free(e);
    mem_free(D);
    return sum;
}
