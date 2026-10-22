/*
 * A354807  a(n) = 1 if A346241(n) is negative, and 0 otherwise, where A346241
 *          is the Dirichlet inverse of {A003415, arithmetic derivative of n +
 *          A063524 (1, 0, 0, 0, ...)}.
 * https://oeis.org/A354807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A346241(n) < 0] (Dirichlet inverse of the sequence whose
 *            n-th term is the arithmetic derivative d(n) with d(1)=1)
 * Range:     u8 n <= 9935, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t  max_n = 0;
static int64_t *g = NULL;   /* g(n) = d(n) for n>=2, g(1)=1  */
static int64_t *f = NULL;   /* f(n) = A346241(n)            */
static uint32_t *spf = NULL;

/* Build all tables up to N (inclusive).  Rebuild if N > max_n. */
static void build_tables(idx_t N)
{
    if (N <= max_n)
        return;

    /* free previous tables */
    free(g); free(f); free(spf);

    size_t cap = (size_t)N + 1;
    g   = (int64_t*)malloc(cap * sizeof(*g));
    f   = (int64_t*)malloc(cap * sizeof(*f));
    spf = (uint32_t*)malloc(cap * sizeof(*spf));

    /* linear sieve for smallest prime factor */
    for (size_t i = 0; i < cap; ++i) spf[i] = 0;
    uint32_t *primes = (uint32_t*)malloc(cap * sizeof(*primes));
    size_t   pcnt   = 0;
    for (uint32_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt && primes[j] <= spf[i] && (uint64_t)i * primes[j] <= N; ++j)
            spf[(uint32_t)(i * primes[j])] = primes[j];
    }
    free(primes);

    /* compute g: derivative. */
    g[1] = 1;
    for (uint32_t n = 2; n <= N; ++n) {
        uint32_t tmp = n;
        int64_t sum = 0;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            uint32_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            sum += (int64_t) e * (int64_t)n / p;
        }
        g[n] = sum;          /* g[n] is positive integer */
    }

    /* compute f: Dirichlet inverse of g */
    f[1] = 1;                /* by definition */
    for (uint32_t n = 2; n <= N; ++n) {
        int64_t sum = 0;
        /* process divisors d > 1, d <= sqrt(n) */
        for (uint32_t d = 2; (uint64_t)d * d <= n; ++d) {
            if (n % d == 0) {
                uint32_t d1 = d;
                uint32_t d2 = n / d;
                sum += g[d1] * f[n / d1];
                if (d2 != d1 && d2 > 1)
                    sum += g[d2] * f[n / d2];
            }
        }
        /* add d = n term: g[n]*f[1] */
        sum += g[n] * f[1];
        /* solve f[n] + sum = 0  =>  f[n] = -sum */
        f[n] = - sum;
    }

    max_n = N;
}

/* Required function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    build_tables(n);
    return (seq_t)(f[n] < 0 ? 1 : 0);
}
