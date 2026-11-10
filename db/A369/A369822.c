/*
 * A369822  Number of (undirected) Eulerian cycles in the (2n)-dipyramid
 *          graph.
 * https://oeis.org/A369822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*(n-1)!*(4^n*Sum_{k=0..n}
 *            binomial(2n,2k)*binomial(2k,k) - binomial(2n,n) -
 *            4*Sum_{q=0..2n-2} binomial(q,floor(q/2))*A193858(2n-2,q)), where
 *            A193858(r,q) = Sum_{j=0..q} 2^(r-j)*binomial(r-j,q-j)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 96 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t N = 2 * n;
    idx_t W = N + 1;
    seq_t *P = mem_new(seq_t, (size_t)(W * W));
    for (idx_t i = 0; i <= N; i++) {
        P[i * W] = 1;
        for (idx_t j = 1; j <= i; j++) {
            P[i * W + j] = P[(i - 1) * W + j - 1] + (j <= i - 1 ? P[(i - 1) * W + j] : (seq_t)0);
        }
    }

    seq_t sum1 = 0;
    for (idx_t k = 0; k <= n; k++)
        sum1 += P[N * W + 2 * k] * P[(2 * k) * W + k];

    seq_t p4 = 1;
    for (idx_t i = 0; i < n; i++) p4 = p4 * 4;

    idx_t r = N - 2;
    seq_t sum2 = 0;
    for (idx_t q = 0; q <= r; q++) {
        seq_t t = 0;
        seq_t pw = 1;
        /* iterate j from q down to 0 so that r-j increases from r-q */
        for (idx_t j = q + 1; j-- > 0;) {
            idx_t m = r - j;
            /* 2^m */
            if (j == q) {
                pw = 1;
                for (idx_t e = 0; e < m; e++) pw = pw * 2;
            } else {
                pw = pw * 2;
            }
            t += pw * P[m * W + (q - j)];
        }
        sum2 += P[q * W + q / 2] * t;
    }

    seq_t br = p4 * sum1 - P[N * W + n] - 4 * sum2;

    seq_t f = 1;
    for (idx_t i = 2; i <= n; i++) f = f * (seq_t)i;      /* n! */
    seq_t g = 1;
    for (idx_t i = 2; i + 1 <= n; i++) g = g * (seq_t)i;  /* (n-1)! */

    seq_t res = f * g * br;
    mem_free(P);
    return res;
}
