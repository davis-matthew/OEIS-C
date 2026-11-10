/*
 * A379433  Number of rooted planar maps with n edges and without faces of
 *          degree 1.
 * https://oeis.org/A379433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. x + G(x/(1+x)^2)*(1-x)/(1+x), G = g.f. of A000168, so *
 *            a(n) = [n=1] + g(n) + Sum_{k=0..n-1} (-1)^(n-k) * g(k) * *
 *            (C(n+k, n-k) + C(n+k-1, n-k-1)), * g(k) = A000168(k) =
 *            2*3^k*(2k)!/(k!(k+2)!), (k+2)*g(k) = 6*(2k-1)*g(k-1), g(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-10)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    long long N = (long long)n;
    long long k, r, j;
    int w = 0, bounded;
    seq_t probe = 1;

    if (n == 0) return 1;

    /* detect a fixed-width seq_t and its bit width */
    while (w <= 300 && probe != 0) { probe += probe; w++; }
    bounded = (probe == 0);

    seq_t *G = mem_new(seq_t, N + 1);
    seq_t *C1 = mem_new(seq_t, N + 2);
    seq_t *B = mem_new(seq_t, N + 2);
    seq_t *R = mem_new(seq_t, N + 2);

    /* g(k) = A000168(k) */
    G[0] = 1;
    if (!bounded) {
        for (k = 1; k <= N; k++) {
            unsigned long long q = 6ULL * (2ULL * (unsigned long long)k - 1ULL);
            unsigned long long d = (unsigned long long)k + 2ULL;
            unsigned long long g = gcd_u64(q, d);
            q /= g;
            d /= g;
            seq_t v = G[k - 1];
            v /= d;
            v *= q;
            G[k] = v;
        }
    } else {
        /* exact modulo 2^w: g = U * 2^E with U odd, divide by odd parts via inverse */
        seq_t U = 1;
        long long E = 0;
        for (k = 1; k <= N; k++) {
            unsigned long long q = 6ULL * (2ULL * (unsigned long long)k - 1ULL);
            unsigned long long d = (unsigned long long)k + 2ULL;
            int it;
            while ((q & 1ULL) == 0) { q >>= 1; E++; }
            while ((d & 1ULL) == 0) { d >>= 1; E--; }
            seq_t D = (seq_t)d;
            seq_t inv = D;
            for (it = 0; it < 8; it++) {
                seq_t t = D * inv;
                t = (seq_t)2 - t;
                inv = inv * t;
            }
            U = U * (seq_t)q;
            U = U * inv;
            seq_t v = 0;
            if (E < (long long)w) {
                long long s;
                v = U;
                for (s = 0; s < E; s++) v += v;
            }
            G[k] = v;
        }
    }

    /* Pascal rows by additions only (exact, or exact mod 2^w), columns 0..N */
    R[0] = 1;
    for (r = 0; r <= 2 * N; r++) {
        if (r > 0) {
            long long top = r < N ? r : N;
            for (j = top; j >= 1; j--) R[j] += R[j - 1];
        }
        if (r >= N) {
            k = r - N;                    /* C(N+k, N-k) */
            if (k <= N) C1[k] = R[N - k];
        }
        if (r >= N - 1) {
            k = r - N + 1;                /* C(N+k-1, N-k-1) */
            if (k >= 0 && N - k - 1 >= 0) B[k] = R[N - k - 1];
        }
    }

    seq_t pos = 0, neg = 0;
    for (k = 0; k <= N; k++) {
        long long m = N - k;
        if (m == 0) {
            pos += G[k];
        } else {
            seq_t c = C1[k] + B[k];
            seq_t t = G[k] * c;
            if (m & 1) neg += t;
            else pos += t;
        }
    }
    if (n == 1) pos += 1;
    seq_t res = pos - neg;

    mem_free(G);
    mem_free(C1);
    mem_free(B);
    mem_free(R);
    return res;
}
