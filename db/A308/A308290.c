/*
 * A308290  Expansion of Sum_{k>=1} mu(k)*log(1 + x^k/(1 - x^k)^3)/k.
 * https://oeis.org/A308290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Inverse Euler transform of the triangular numbers (OEIS A308290)
 *            a(1) = 1 For n>1: a(n) = (v(n) - sum_{d|n, d<n} d*a(d)) / n
 *            where v(n) = n*u(n) - sum_{k=1}^{n-1} u(n-k)*v(k) and u(n) =
 *            n*(n+1)/2 (triangular numbers)
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   divisor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the build. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_sum.h"

static size_t g_max_n = 0;
static seq_t   *g_u   = NULL;   /* triangular numbers */
static seq_t   *g_v   = NULL;   /* v(n) */
static seq_t   *g_a   = NULL;   /* a(n) */
static idx_t   *g_spf = NULL;   /* smallest prime factor table */

/* Ensure arrays are large enough and fill them up to n. */
static void
ensure_capacity(idx_t n)
{
    if (n <= g_max_n)
        return;

    size_t new_cap = (size_t)n + 1;
    g_u   = (seq_t*)realloc(g_u,   new_cap * sizeof(seq_t));
    g_v   = (seq_t*)realloc(g_v,   new_cap * sizeof(seq_t));
    g_a   = (seq_t*)realloc(g_a,   new_cap * sizeof(seq_t));
    g_spf = (idx_t*)realloc(g_spf, new_cap * sizeof(idx_t));

    /* initialise new entries (0 is fine) */
    for (size_t i = g_max_n + 1; i < new_cap; ++i) {
        g_u[i] = 0;
        g_v[i] = 0;
        g_a[i] = 0;
    }

    /* compute smallest prime factor sieve up to n */
    for (idx_t i = 0; i < new_cap; ++i)
        g_spf[i] = i;
    for (idx_t i = 2; i * i < new_cap; ++i)
        if (g_spf[i] == i)
            for (idx_t j = i * i; j < new_cap; j += i)
                if (g_spf[j] == j)
                    g_spf[j] = i;

    /* precompute triangular numbers */
    for (idx_t i = 1; i <= n; ++i)
        g_u[i] = (seq_t)i * (seq_t)(i + 1) / 2;

    /* compute v up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            g_v[1] = g_u[1];
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k)
                sum += g_u[i - k] * g_v[k];
            g_v[i] = (seq_t)i * g_u[i] - sum;
        }
    }

    /* compute a up to n using divisor sums */
    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            g_a[1] = g_v[1];
        } else {
            /* factorise i using spf */
            idx_t tmp = i;
            idx_t primes[16];
            int  exps[16];
            size_t pcnt = 0;
            while (tmp > 1) {
                idx_t p = g_spf[tmp];
                int e = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++e;
                }
                primes[pcnt] = p;
                exps[pcnt]  = e;
                ++pcnt;
            }

            /* generate all divisors */
            idx_t divs[256];
            size_t divcnt = 1;
            divs[0] = 1;
            for (size_t t = 0; t < pcnt; ++t) {
                idx_t p = primes[t];
                int e  = exps[t];
                size_t old = divcnt;
                idx_t mult = 1;
                for (int ex = 1; ex <= e; ++ex) {
                    mult *= p;
                    for (size_t j = 0; j < old; ++j)
                        divs[divcnt++] = divs[j] * mult;
                }
            }

            seq_t sumd = 0;
            for (size_t j = 0; j < divcnt; ++j) {
                idx_t d = divs[j];
                if (d < i)
                    sumd += (seq_t)d * g_a[d];
            }
            g_a[i] = (g_v[i] - sumd) / (seq_t)i;
        }
    }

    g_max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined, but return 0 */
    ensure_capacity(n);
    return g_a[n];
}
