/*
 * A328534  G.f. = Phi^3*F^3, where Phi = g.f. for A028930, F = g.f. for
 *          A028959.
 * https://oeis.org/A328534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = Phi^3*F^3, Phi = Sum_{x,y} q^(2x^2+xy+3y^2) (A028930), F
 *            = Sum_{x,y} q^(x^2+xy+6y^2) (A028959); series product truncated
 *            at q^n
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void A328534_mul(const seq_t *p, const seq_t *q, seq_t *r, long long N)
{
    for (long long i = 0; i <= N; i++) {
        seq_t s = 0;
        for (long long j = 0; j <= i; j++)
            s += p[j] * q[i - j];
        r[i] = s;
    }
}

seq_t a(idx_t n)
{
    long long N = (long long)n;
    long long s = 0;
    while (s * s <= N) s++;
    long long B = 2 * s + 2;

    seq_t *P = mem_new(seq_t, N + 1);
    seq_t *F = mem_new(seq_t, N + 1);
    seq_t *G = mem_new(seq_t, N + 1);
    seq_t *H = mem_new(seq_t, N + 1);

    for (long long x = -B; x <= B; x++) {
        for (long long y = -B; y <= B; y++) {
            long long v1 = 2 * x * x + x * y + 3 * y * y;
            if (v1 <= N) P[v1] += 1;
            long long v2 = x * x + x * y + 6 * y * y;
            if (v2 <= N) F[v2] += 1;
        }
    }

    A328534_mul(P, F, G, N);   /* G = Phi*F */
    A328534_mul(G, G, H, N);   /* H = G^2 */
    A328534_mul(H, G, P, N);   /* P = G^3 */

    seq_t res = P[N];
    mem_free(P);
    mem_free(F);
    mem_free(G);
    mem_free(H);
    return res;
}
