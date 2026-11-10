/*
 * A323882  Sum of A126760 and its Dirichlet inverse.
 * https://oeis.org/A323882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A126760(n) + A323881(n), where A323881 is the Dirichlet
 *            inverse g of A126760 = f: g(1)=1, g(n) = -Sum_{d|n, d>1}
 *            f(d)*g(n/d). Computed over the divisors of n
 * Range:     u8 n <= 594, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9402 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A126760_a(idx_t n)
{
    seq_t result = 0;
    idx_t m = n;
    while (m > 0) {
        idx_t r = m % 6;
        if (r == 1) { result = ((seq_t)2 * ((seq_t)(m - 1) / 6)) + 1; break; }
        if (r == 5) { result = ((seq_t)2 * ((seq_t)(m - 5) / 6)) + 2; break; }
        if ((m & 1) == 0) { m >>= 1; continue; }
        if (m % 3 == 0) { m /= 3; continue; }
        break;
    }
    return result;
}

static int A323882_find(const idx_t *d, int k, idx_t v)
{
    int lo = 0, hi = k - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (d[mid] < v) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static int A323882_cmp(const void *x, const void *y)
{
    idx_t a = *(const idx_t *)x, b = *(const idx_t *)y;
    return (a > b) - (a < b);
}

seq_t a(idx_t n)
{
    int cnt = 0, k, i, j;
    idx_t q;
    idx_t *d;
    seq_t *fv, *mag;
    unsigned char *sg;
    seq_t res;

    for (q = 1; q * q <= n; q++)
        if (n % q == 0) cnt += (q * q == n) ? 1 : 2;

    d = (idx_t *)malloc(sizeof(idx_t) * (size_t)cnt);
    sg = (unsigned char *)malloc((size_t)cnt);
    fv = mem_new(seq_t, cnt);
    mag = mem_new(seq_t, cnt);

    k = 0;
    for (q = 1; q * q <= n; q++)
        if (n % q == 0) {
            d[k++] = q;
            if (q * q != n) d[k++] = n / q;
        }
    qsort(d, (size_t)k, sizeof(idx_t), A323882_cmp);

    for (i = 0; i < k; i++) fv[i] = A126760_a(d[i]);

    mag[0] = 1; sg[0] = 0;
    for (i = 1; i < k; i++) {
        seq_t P = 0, N = 0;
        for (j = 1; j <= i; j++) {
            if (d[i] % d[j] == 0) {
                int idx = A323882_find(d, k, d[i] / d[j]);
                seq_t t = fv[j] * mag[idx];
                if (sg[idx]) N += t; else P += t;
            }
        }
        /* g = -(P - N) */
        if (N >= P) { mag[i] = N - P; sg[i] = 0; }
        else { mag[i] = P - N; sg[i] = 1; }
    }

    if (sg[k - 1]) res = fv[k - 1] - mag[k - 1];
    else res = fv[k - 1] + mag[k - 1];

    free(d); free(sg);
    mem_free(fv); mem_free(mag);
    return res;
}
