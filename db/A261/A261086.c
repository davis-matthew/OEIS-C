/*
 * A261086  Numbers n > 1 for which A261085(n) < A261085(n-1).
 * https://oeis.org/A261086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m>1 with A261085(m) < A261085(m-1), where A261085(m)
 *            = f(prime(m)), f(0)=0, f(k)=1+f(k-d(k)), d = number of divisors;
 *            computed by a sieve with doubling limit
 * Range:     u8 n <= 19, u16 n <= 9417, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9248 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A261086_res = NULL;
static size_t A261086_cnt = 0;
static size_t A261086_lim = 0;

static void A261086_build(size_t L)
{
    uint16_t *d = (uint16_t *)calloc(L + 1, sizeof(uint16_t));
    uint32_t *f = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)malloc((L / 2 + 4) * sizeof(uint32_t));
    size_t i, j, cnt = 0;
    uint32_t m = 0;
    uint32_t prev = 0;
    if (!d || !f || !r) abort();
    for (i = 1; i <= L; ++i)
        for (j = i; j <= L; j += i) d[j]++;
    f[0] = 0;
    for (i = 1; i <= L; ++i) {
        f[i] = 1 + f[i - d[i]];
        if (d[i] == 2) {
            ++m;
            if (m >= 2 && f[i] < prev) r[cnt++] = m;
            prev = f[i];
        }
    }
    free(d);
    free(f);
    free(A261086_res);
    A261086_res = r;
    A261086_cnt = cnt;
    A261086_lim = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A261086_lim == 0) A261086_build(1 << 16);
    while ((idx_t)A261086_cnt < n) A261086_build(A261086_lim * 2);
    return (seq_t)A261086_res[(size_t)(n - 1)];
}
