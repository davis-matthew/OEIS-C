/*
 * A349354  Sum of A328203 and its Dirichlet inverse.
 * https://oeis.org/A349354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A328203(n) + A349353(n), where A349353 is the Dirichlet
 *            inverse of A328203: g(1) = 1, g(n) = -Sum_{d|n, d>1}
 *            A328203(d)*g(n/d); A328203(2n) = 2*A328203(n), odd n:
 *            (n*d(n)+sigma(n))/2
 * Range:     u8 n <= 50, u16 n <= 5312, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 95 terms through n = 9817 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static void A328203_div_sigma(idx_t n, seq_t *divs, seq_t *sg)
{
    idx_t remaining = n;
    seq_t d = 1;
    seq_t s = 1;
    for (idx_t p = 2; p * p <= remaining; ++p)
    {
        if (remaining % p == 0)
        {
            int cnt = 0;
            seq_t pw = 1;
            seq_t sum = 1;
            while (remaining % p == 0)
            {
                remaining /= p;
                ++cnt;
                pw *= (seq_t)p;
                sum += pw;
            }
            d *= (seq_t)(cnt + 1);
            s *= sum;
        }
    }
    if (remaining > 1)
    {
        d *= 2;
        s *= (1 + (seq_t)remaining);
    }
    *divs = d;
    *sg = s;
}

static seq_t A328203_a(idx_t n)
{
    if ((n & 1) == 0)
        return 2 * A328203_a(n >> 1);
    seq_t d, sigma_val;
    A328203_div_sigma(n, &d, &sigma_val);
    return ((seq_t)n * d + sigma_val) / 2;
}

static int A349354_cmp(const void *x, const void *y)
{
    idx_t p = *(const idx_t *)x, q = *(const idx_t *)y;
    return (p > q) - (p < q);
}

static size_t A349354_find(const idx_t *v, size_t k, idx_t x)
{
    size_t lo = 0, hi = k;
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (v[mid] < x) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    size_t cap = 64, k = 0;
    idx_t *dv = (idx_t *)malloc(cap * sizeof(idx_t));
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            if (k + 2 > cap)
            {
                cap *= 2;
                dv = (idx_t *)realloc(dv, cap * sizeof(idx_t));
            }
            dv[k++] = i;
            if (i != n / i) dv[k++] = n / i;
        }
    }
    qsort(dv, k, sizeof(idx_t), A349354_cmp);

    seq_t *fv = mem_new(seq_t, k);
    seq_t *gv = mem_new(seq_t, k);
    for (size_t i = 0; i < k; ++i)
        fv[i] = A328203_a(dv[i]);

    gv[0] = 1;
    for (size_t i = 1; i < k; ++i)
    {
        seq_t sum = 0;
        for (size_t j = 1; j <= i; ++j)
        {
            if (dv[i] % dv[j] == 0)
            {
                size_t q = A349354_find(dv, k, dv[i] / dv[j]);
                sum += fv[j] * gv[q];
            }
        }
        gv[i] = (seq_t)0 - sum;
    }
    seq_t res = fv[k - 1] + gv[k - 1];
    mem_free(fv);
    mem_free(gv);
    free(dv);
    return res;
}
