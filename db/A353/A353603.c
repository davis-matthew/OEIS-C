/*
 * A353603  Number of graph minors in the n-pan graph.
 * https://oeis.org/A353603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(n-2) + A000070(n+1) - 1 + Sum_{j=0..n-3}
 *            floor((n-j-1)/2) * A000070(j), where A000070(m) = Sum_{k=0..m}
 *            A000041(k)
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 83, u64 n <= 307,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 931 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t m, k, j;
    idx_t N;
    seq_t *P, *S;
    seq_t res;
    if (n < 2) return 0;
    N = n + 1;
    P = mem_new(seq_t, (size_t)N + 1);
    S = mem_new(seq_t, (size_t)N + 1);
    P[0] = 1;
    S[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) {
                pos += P[m - g1];
                if (g2 <= m) pos += P[m - g2];
            } else {
                neg += P[m - g1];
                if (g2 <= m) neg += P[m - g2];
            }
        }
        P[m] = pos - neg;
        S[m] = S[m - 1] + P[m];
    }
    res = (seq_t)3 * (seq_t)(n - 2) + S[N] - 1;
    if (n >= 3) {
        for (j = 0; j <= n - 3; j++) {
            res += (seq_t)((n - j - 1) / 2) * S[j];
        }
    }
    mem_free(P);
    mem_free(S);
    return res;
}
