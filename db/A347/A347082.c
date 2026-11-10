/*
 * A347082  Dirichlet inverse of -A168036, n - A003415(n).
 * https://oeis.org/A347082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n > 1, a(n) = Sum_{d|n, d<n} a(d) * A168036(n/d),
 *            where A168036(m) = m' - m
 * Range:     i8 n <= 127, i16 n <= 4095, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 95 terms through n = 8989 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A347082_tab = 0;
static idx_t A347082_cap = 0;

static void A347082_build(idx_t N)
{
    idx_t i, d, m;
    seq_t *der, *acc;
    idx_t *spf;

    if (A347082_tab) {
        mem_free(A347082_tab);
        A347082_tab = 0;
    }
    der = mem_new(seq_t, N + 1);
    acc = mem_new(seq_t, N + 1);
    spf = mem_new(idx_t, N + 1);

    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (m = i; m <= N; m += i)
                if (spf[m] == 0) spf[m] = i;
        }
    }
    der[0] = 0;
    if (N >= 1) der[1] = 0;
    for (i = 2; i <= N; i++) {
        idx_t p = spf[i];
        idx_t q = i / p;
        /* (p*q)' = q + p*q' */
        der[i] = (seq_t)q + (seq_t)p * der[q];
    }
    /* der[m] becomes A168036(m) = m' - m */
    for (i = 1; i <= N; i++)
        der[i] = der[i] - (seq_t)i;

    acc[1] = 1;
    for (d = 1; d <= N; d++) {
        seq_t ad = acc[d];
        for (m = 2; d * m <= N; m++)
            acc[d * m] += ad * der[m];
    }
    mem_free(der);
    mem_free(spf);
    A347082_tab = acc;
    A347082_cap = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > A347082_cap || !A347082_tab) {
        idx_t N = n < 2048 ? 2048 : n;
        A347082_build(N);
    }
    return A347082_tab[n];
}
