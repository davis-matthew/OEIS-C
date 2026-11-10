/*
 * A328528  G.f. = Phi^3, where Phi = g.f. for A028930.
 * https://oeis.org/A328528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = Phi(x)^3 where Phi = sum_{x,y in Z} q^(2x^2+xy+3y^2)
 *            (A028930); coefficient extraction by direct convolution
 * Range:     u8 n <= 15, u16 n <= 221, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 65 terms through n = 4912 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static void A328528_phi(idx_t n, seq_t *p)
{
    long long N = (long long)n;
    long long L = 0;
    while ((L + 1) * (L + 1) <= N) L++;
    L++;
    for (long long y = -L; y <= L; ++y)
        for (long long x = -L; x <= L; ++x) {
            long long v = 2 * x * x + x * y + 3 * y * y;
            if (v <= N) p[v]++;
        }
}

static void A328528_mul(const seq_t *u, const seq_t *v, seq_t *r, long long N)
{
    for (long long i = 0; i <= N; ++i) {
        seq_t s = 0;
        for (long long j = 0; j <= i; ++j)
            s += u[j] * v[i - j];
        r[i] = s;
    }
}

seq_t a(idx_t n)
{
    long long N = (long long)n;
    seq_t *p = mem_new(seq_t, N + 1);
    seq_t *q = mem_new(seq_t, N + 1);
    seq_t *r = mem_new(seq_t, N + 1);
    A328528_phi(n, p);
    A328528_mul(p, p, q, N);
    /* only the last coefficient of q*p is needed */
    seq_t s = 0;
    for (long long j = 0; j <= N; ++j)
        s += q[j] * p[N - j];
    (void)r;
    mem_free(p);
    mem_free(q);
    mem_free(r);
    return s;
}
