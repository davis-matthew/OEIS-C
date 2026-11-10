/*
 * A368429  Main diagonal of A357914.
 * https://oeis.org/A368429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n,n), where T(1,k) = mu(k) and T(r,k) = Sum_{i=1..k}
 *            T(r-1,i) (iterated partial sums of Moebius mu)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 969 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;
    int *mu = (int *)malloc((N + 1) * sizeof(int));
    for (size_t i = 0; i <= N; i++) mu[i] = 1;
    char *comp = (char *)calloc(N + 1, 1);
    /* mu sieve: start with 1, flip sign per prime, zero for squares */
    for (size_t p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (size_t m = p; m <= N; m += p) {
            if (m > p) comp[m] = 1;
            mu[m] = -mu[m];
        }
        if (p <= N / p) {
            size_t pp = p * p;
            for (size_t m = pp; m <= N; m += pp) mu[m] = 0;
        }
    }
    /* positive and negative parts kept separately so values stay nonnegative */
    seq_t *P = mem_new(seq_t, N + 1);
    seq_t *Q = mem_new(seq_t, N + 1);
    for (size_t j = 1; j <= N; j++) {
        if (mu[j] > 0) P[j] = 1;
        else if (mu[j] < 0) Q[j] = 1;
    }
    for (size_t r = 2; r <= N; r++) {
        seq_t cp = 0, cq = 0;
        for (size_t j = 1; j <= N; j++) {
            cp += P[j]; P[j] = cp;
            cq += Q[j]; Q[j] = cq;
        }
    }
    seq_t res = P[N] - Q[N];
    mem_free(P);
    mem_free(Q);
    free(mu);
    free(comp);
    return res;
}
