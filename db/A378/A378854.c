/*
 * A378854  a(n) is the number of solid (3D) partitions of n with 2 layers and
 *          second layer a plane partition of 2.
 * https://oeis.org/A378854
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(A000219(n-2) - A000041(n-2)), where A000219 = plane
 *            partitions (G.f. Product 1/(1-x^k)^k) and A000041 = partitions
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 38 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A378854_P = NULL;
static seq_t *A378854_p = NULL;
static size_t A378854_cap = 0;

static uint64_t A378854_sigma2(uint64_t k)
{
    uint64_t s = 0, d;
    for (d = 1; d * d <= k; d++) {
        if (k % d == 0) {
            uint64_t e = k / d;
            s += d * d;
            if (e != d) s += e * e;
        }
    }
    return s;
}

static void A378854_build(size_t N)
{
    size_t m, k, j, r, L;
    seq_t *P = mem_new(seq_t, N + 1);
    seq_t *q = mem_new(seq_t, N + 1);
    L = N < 400 ? N : 400;
    P[0] = 1;
    for (k = 1; k <= L; k++)
        for (r = 0; r < k; r++)
            for (j = k; j <= L; j++)
                P[j] += P[j - k];
    for (m = L + 1; m <= N; m++) {
        seq_t s = 0;
        for (k = 1; k <= m; k++)
            s += P[m - k] * (seq_t)A378854_sigma2((uint64_t)k);
        P[m] = s / (seq_t)m;
    }
    q[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            size_t g1 = k * (3 * k - 1) / 2;
            size_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) {
                pos += q[m - g1];
                if (g2 <= m) pos += q[m - g2];
            } else {
                neg += q[m - g1];
                if (g2 <= m) neg += q[m - g2];
            }
        }
        q[m] = pos - neg;
    }
    if (A378854_P) mem_free(A378854_P);
    if (A378854_p) mem_free(A378854_p);
    A378854_P = P;
    A378854_p = q;
    A378854_cap = N + 1;
}

seq_t a(idx_t n)
{
    size_t m = (size_t)(n - 2);
    if (m + 1 > A378854_cap) {
        size_t cap = A378854_cap ? A378854_cap : 64;
        while (cap < m + 1) cap *= 2;
        A378854_build(cap - 1);
    }
    return (seq_t)3 * (A378854_P[m] - A378854_p[m]);
}
