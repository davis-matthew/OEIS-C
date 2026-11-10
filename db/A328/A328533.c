/*
 * A328533  G.f. = Phi^4*F^2, where Phi = g.f. for A028930, F = g.f. for
 *          A028959.
 * https://oeis.org/A328533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = Phi^4*F^2, Phi = Sum_{x,y} q^(2x^2+xy+3y^2) (A028930), F
 *            = Sum_{x,y} q^(x^2+xy+6y^2) (A028959); coefficients by direct
 *            enumeration and polynomial multiplication
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void A328533_theta(long long N, int ca, int cb, int cc, seq_t *arr)
{
    long long L = 0;
    while (L * L <= N) L++;
    L++;
    for (long long x = -L; x <= L; ++x)
        for (long long y = -L; y <= L; ++y) {
            long long v = ca * x * x + cb * x * y + cc * y * y;
            if (v >= 0 && v <= N) arr[v] += 1;
        }
}

static seq_t *A328533_mul(const seq_t *p, const seq_t *q, long long N)
{
    seq_t *r = mem_new(seq_t, N + 1);
    for (long long i = 0; i <= N; ++i) {
        if (p[i] == 0) continue;
        for (long long j = 0; i + j <= N; ++j)
            r[i + j] += p[i] * q[j];
    }
    return r;
}

seq_t a(idx_t n)
{
    long long N = (long long)n;
    seq_t *phi = mem_new(seq_t, N + 1);
    seq_t *f = mem_new(seq_t, N + 1);
    A328533_theta(N, 2, 1, 3, phi);
    A328533_theta(N, 1, 1, 6, f);

    seq_t *p2 = A328533_mul(phi, phi, N);
    seq_t *p4 = A328533_mul(p2, p2, N);
    seq_t *f2 = A328533_mul(f, f, N);

    seq_t res = 0;
    for (long long i = 0; i <= N; ++i)
        res += p4[i] * f2[N - i];

    mem_free(phi);
    mem_free(f);
    mem_free(p2);
    mem_free(p4);
    mem_free(f2);
    return res;
}
