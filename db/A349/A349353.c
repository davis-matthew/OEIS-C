/*
 * A349353  Dirichlet inverse of A328203.
 * https://oeis.org/A349353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = -Sum_{d|n, d<n} A328203(n/d) * a(d), where
 *            A328203(2n)=2*A328203(n), A328203(odd n)=(n*tau(n)+sigma(n))/2
 * Range:     i8 n <= 56, i16 n <= 8609, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 91 terms through n = 7521 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
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

static seq_t A349353_cache_dummy = 0;
static seq_t *A349353_a = NULL;
static idx_t A349353_cap = 0;

static void A349353_build(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *f = mem_new(seq_t, N + 1);
    seq_t *acc = mem_new(seq_t, N + 1);
    seq_t *res = mem_new(seq_t, N + 1);

    for (size_t k = 1; k <= N; ++k)
    {
        if ((k & 1) == 0)
            f[k] = 2 * f[k >> 1];
        else
        {
            seq_t d, s;
            A328203_div_sigma((idx_t)k, &d, &s);
            f[k] = ((seq_t)k * d + s) / 2;
        }
    }

    for (size_t d = 1; d <= N; ++d)
    {
        if (d == 1)
            res[d] = 1;
        else
            res[d] = -acc[d];
        if (res[d] == 0)
            continue;
        for (size_t m = 2 * d, q = 2; m <= N; m += d, ++q)
            acc[m] += f[q] * res[d];
    }

    mem_free(f);
    mem_free(acc);
    if (A349353_a != NULL)
        mem_free(A349353_a);
    A349353_a = res;
    A349353_cap = n;
}

seq_t a(idx_t n)
{
    (void)A349353_cache_dummy;
    if (n == 0)
        return 0;
    if (A349353_a == NULL || n > A349353_cap)
    {
        idx_t want = n;
        if (want < 1024)
            want = 1024;
        A349353_build(want);
    }
    return A349353_a[(size_t)n];
}
