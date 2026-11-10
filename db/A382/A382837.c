/*
 * A382837  Numbers k such that k - A071324(k) > A000010(k).
 * https://oeis.org/A382837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that k - A071324(k) > A000010(k); computed by
 *            a doubling sieve of tau, phi and the alternating divisor sum
 * Range:     u8 n <= 13, u16 n <= 5433, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9022 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static long long *oeis_terms = 0;
static long long oeis_cnt = 0;
static long long oeis_lim = 0;

static void oeis_build(long long L)
{
    long long *tau = (long long *)calloc((size_t)(L + 1), sizeof(long long));
    long long *cnt = (long long *)calloc((size_t)(L + 1), sizeof(long long));
    long long *sum = (long long *)calloc((size_t)(L + 1), sizeof(long long));
    long long *phi = (long long *)malloc((size_t)(L + 1) * sizeof(long long));
    long long d, k;
    for (k = 0; k <= L; k++) phi[k] = k;
    for (d = 2; d <= L; d++) {
        if (phi[d] == d) {
            for (k = d; k <= L; k += d) phi[k] -= phi[k] / d;
        }
    }
    for (d = 1; d <= L; d++)
        for (k = d; k <= L; k += d) tau[k]++;
    for (d = 1; d <= L; d++) {
        for (k = d; k <= L; k += d) {
            long long pos = tau[k] - cnt[k];
            cnt[k]++;
            if (pos & 1) sum[k] += d; else sum[k] -= d;
        }
    }
    free(oeis_terms);
    oeis_terms = (long long *)malloc((size_t)(L + 1) * sizeof(long long));
    oeis_cnt = 0;
    for (k = 1; k <= L; k++) {
        if (k - sum[k] > phi[k]) oeis_terms[oeis_cnt++] = k;
    }
    oeis_lim = L;
    free(tau); free(cnt); free(sum); free(phi);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (oeis_lim == 0) oeis_build(1 << 16);
    while ((unsigned long long)oeis_cnt < (unsigned long long)n)
        oeis_build(oeis_lim * 2);
    return (seq_t)oeis_terms[(size_t)(n - 1)];
}
