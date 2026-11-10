/*
 * A362586  Triangle read by rows, T(n, k) = A094088(n) * binomial(n, k).
 * https://oeis.org/A362586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A094088(n)*binomial(n,k), with A094088(0)=1, A094088(n)
 *            = Sum_{k=1..n} binomial(2n,2k)*A094088(n-k); triangle read by
 *            rows, offset 0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mem.h"

static seq_t T362586_A094088(size_t r)
{
    seq_t *a = mem_new(seq_t, r + 1);
    seq_t *P = mem_new(seq_t, 2 * r + 2);
    seq_t res;
    size_t i, j, k, row = 0;
    a[0] = 1;
    P[0] = 1;
    for (i = 1; i <= r; i++) {
        for (int s = 0; s < 2; s++) {
            row++;
            for (j = row; j >= 1; j--) P[j] += P[j - 1];
        }
        seq_t sum = 0;
        for (k = 1; k <= i; k++) sum += P[2 * k] * a[i - k];
        a[i] = sum;
    }
    res = a[r];
    mem_free(a);
    mem_free(P);
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = 0, m = n;
    while (m > r) { m -= r + 1; r++; }
    size_t k = (size_t)m, R = (size_t)r;

    seq_t A = T362586_A094088(R);

    /* binomial(R,k) via Pascal row */
    seq_t *B = mem_new(seq_t, R + 2);
    B[0] = 1;
    for (size_t i = 1; i <= R; i++)
        for (size_t j = i; j >= 1; j--) B[j] += B[j - 1];
    seq_t c = B[k];
    mem_free(B);
    return A * c;
}
