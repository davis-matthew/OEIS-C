/*
 * A259205  E.g.f.: Sum_{n>=0} x^n * (2 + x^n)^n * exp(3*x^(n+1)) / n!.
 * https://oeis.org/A259205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A259205: e.g.f. Sum_{k>=0} x^k (2 + x^k)^k exp(3 x^(k+1)) / k!. Taking the
 * coefficient of x^n,
 *   a(n) = Sum C(n,k) C(n-k,j) (n-k-j)! C(k,i) 2^(k-i) 3^j
 * over k(1+i) + j(k+1) = n, since n!/(k! j!) = C(n,k) C(n-k,j) (n-k-j)!.
 * Pascal's triangle and plain products only, so exact modulo the width (the
 * original divided by factorials after they had wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    seq_t *C = mem_new(seq_t, (N + 1) * (N + 1));  /* C[r (N+1) + c] */
    seq_t *F = mem_new(seq_t, N + 1), *P2 = mem_new(seq_t, N + 1), *P3 = mem_new(seq_t, N + 1);
    if (!C || !F || !P2 || !P3) abort();
    F[0] = P2[0] = P3[0] = (seq_t)1;
    for (size_t r = 0; r <= N; ++r) {
        if (r) { F[r] = F[r - 1] * (seq_t)r; P2[r] = P2[r - 1] * (seq_t)2; P3[r] = P3[r - 1] * (seq_t)3; }
        C[r * (N + 1)] = (seq_t)1;
        for (size_t c = 1; c <= r; ++c)
            C[r * (N + 1) + c] = C[(r - 1) * (N + 1) + c - 1] + (c < r ? C[(r - 1) * (N + 1) + c] : (seq_t)0);
    }
    seq_t s = (seq_t)0;
    for (size_t k = 0; k <= N; ++k)
        for (size_t i = 0; i <= k; ++i) {
            size_t used = k * (1 + i);
            if (used > N) break;
            size_t rest = N - used;
            if (rest % (k + 1)) continue;
            size_t j = rest / (k + 1);
            if (k + j > N) continue;
            s += C[N * (N + 1) + k] * C[(N - k) * (N + 1) + j] * F[N - k - j]
                 * C[k * (N + 1) + i] * P2[k - i] * P3[j];
        }
    mem_free(C); mem_free(F); mem_free(P2); mem_free(P3);
    return s;
}
