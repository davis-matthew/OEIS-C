/*
 * A274355  Convolution of A048272 and A022567.
 * https://oeis.org/A274355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A048272(k)*A022567(n-k), where A048272(k) =
 *            Sum_{d|k} (-1)^(d+1) and A022567(m) = Sum_{T_j<=m} p(m-T_j) with
 *            p the partition numbers (pentagonal recurrence)
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 101, u64 n <= 354,
 *            u128 n <= 1313, big any n
 * Validated: standard, pass, 57 terms through n = 9510 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N == 0) return 0;

    /* partition numbers p[0..N] */
    seq_t *p = mem_new(seq_t, N + 1);
    p[0] = 1;
    for (size_t cur = 1; cur <= N; ++cur) {
        seq_t so = 0, se = 0;
        for (size_t k = 1;; ++k) {
            size_t g1 = k * (3 * k - 1) / 2;
            if (g1 > cur) break;
            if (k & 1) so += p[cur - g1]; else se += p[cur - g1];
            size_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= cur) {
                if (k & 1) so += p[cur - g2]; else se += p[cur - g2];
            }
        }
        p[cur] = so - se;
    }

    /* A022567 values q[0..N-1] */
    seq_t *q = mem_new(seq_t, N);
    for (size_t m = 0; m < N; ++m) {
        seq_t s = 0;
        for (size_t k = 0;; ++k) {
            size_t t = k * (k + 1) / 2;
            if (t > m) break;
            s += p[m - t];
        }
        q[m] = s;
    }

    /* A048272 via sieve: sum over d|k of (+1 if d odd else -1) */
    int *w = (int *)calloc(N + 1, sizeof(int));
    for (size_t d = 1; d <= N; ++d) {
        int sg = (d & 1) ? 1 : -1;
        for (size_t k = d; k <= N; k += d) w[k] += sg;
    }

    seq_t pos = 0, neg = 0;
    for (size_t k = 1; k <= N; ++k) {
        int c = w[k];
        if (c > 0) pos += (seq_t)c * q[N - k];
        else if (c < 0) neg += (seq_t)(-c) * q[N - k];
    }

    free(w);
    mem_free(q);
    mem_free(p);
    return pos - neg;
}
