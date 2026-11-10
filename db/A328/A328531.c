/*
 * A328531  G.f. = Phi^6, where Phi = g.f. for A028930.
 * https://oeis.org/A328531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = Phi(x)^6, where Phi(x) = Sum_{x,y in Z} q^(2x^2+xy+3y^2)
 *            is the g.f. of A028930; coefficients by direct convolution
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 159, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 52 terms through n = 4940 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A328531_conv(const seq_t *p, const seq_t *q, long long N)
{
    seq_t *r = mem_new(seq_t, (size_t)(N + 1));
    for (long long i = 0; i <= N; i++) {
        if (p[i] == 0) continue;
        for (long long j = 0; i + j <= N; j++) {
            r[i + j] += p[i] * q[j];
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    long long N = (long long)n;
    seq_t *P = mem_new(seq_t, (size_t)(N + 1));

    long long xl = 0, yl = 0;
    while (23LL * xl * xl <= 12LL * N) xl++;
    while (23LL * yl * yl <= 8LL * N) yl++;

    for (long long y = -yl; y <= yl; y++) {
        for (long long x = -xl; x <= xl; x++) {
            long long v = 2LL * x * x + x * y + 3LL * y * y;
            if (v >= 0 && v <= N) P[v]++;
        }
    }

    seq_t *P2 = A328531_conv(P, P, N);
    seq_t *P3 = A328531_conv(P2, P, N);

    seq_t res = 0;
    for (long long k = 0; k <= N; k++) {
        res += P3[k] * P3[N - k];
    }

    mem_free(P);
    mem_free(P2);
    mem_free(P3);
    return res;
}
