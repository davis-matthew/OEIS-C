/*
 * A318365  Expansion of e.g.f. exp(x*exp(-x)/(1 - x)).
 * https://oeis.org/A318365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n}
 *            A000240(k)*binomial(n-1,k-1)*a(n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *R = mem_new(seq_t, N + 2);   /* A000240 values */
    seq_t *av = mem_new(seq_t, N + 2);  /* sequence values */
    seq_t *row = mem_new(seq_t, N + 2); /* Pascal row */
    size_t i, m, k, j;
    seq_t res;

    R[1] = 1;
    for (i = 2; i <= N; i++) {
        R[i] = R[i - 1] * (seq_t)i;
        if (i & 1)
            R[i] += (seq_t)i;
        else
            R[i] -= (seq_t)i;
    }
    av[0] = 1;
    row[0] = 1;
    for (m = 1; m <= N; m++) {
        if (m > 1) {
            for (j = m - 1; j >= 1; j--)
                row[j] += row[j - 1];
        }
        seq_t s = 0;
        for (k = 1; k <= m; k++)
            s += R[k] * row[k - 1] * av[m - k];
        av[m] = s;
    }
    res = av[N];
    mem_free(R);
    mem_free(av);
    mem_free(row);
    return res;
}
